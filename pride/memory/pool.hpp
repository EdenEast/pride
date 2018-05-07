
#pragma once

// #include <cstddef>
// #include <utility>
#include <memory>
#include <iostream>

namespace pride { namespace memory
{
    template<typename Type = char, std::size_t ObjectsPerBlock = 256>
    class memory_pool
    {
    public:
        using value_type = Type;
        using pointer = value_type*;
        using reference = value_type&;
        using const_pointer = const value_type*;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using void_ptr = void*;

    private:
        struct slot_t
        {
            union
            {
                value_type data;
                slot_t* next;
            };
            uint8_t free;

            static slot_t* get_slot(void_ptr ptr)
            {
                return reinterpret_cast<slot_t>(reinterpret_cast<size_type>(ptr));
            }
        };

        struct block_t
        {
            uint32_t count = 0;
            uint32_t max = 0;
            block_t* next = nullptr;
            block_t* prev = nullptr;
            slot_t* data;
        };

    public:
        memory_pool() = default;
        // memory_pool(size_type reserve)
        // : object_count(0), block_count(0), first_block(nullptr), first_free(nullptr)
        // {
        //     uint32_t n_blocks = reserve / ObjectsPerBlock;
        //     if (n_blocks < 0 || reserve < 0)
        //         return;

        //     for (uint32_t i = 0 ; i < n_blocks ; ++i)
        //         allocate_block();
        // }

        ~memory_pool()
        {
            cleanup();
        }

        memory_pool(const memory_pool&) = delete;
        memory_pool(memory_pool&&) = delete;
        memory_pool& operator=(const memory_pool&) = delete;
        memory_pool& operator=(memory_pool&&) = delete;

        // Allocate the raw memory for 'n' number of 'Type' objects
        pointer allocate()
        {
            // If there is something in the free list then use that
            if (!first_free)
            {
                // auto into = std::get_temporary_buffer<uint8_t>(sizeof(block_t) + ObjectsPerBlock + sizeof(slot_t))
                std::cout << "size: " << sizeof(block_t) + ObjectsPerBlock + sizeof(slot_t) << '\n';

                if (!allocate_block())
                    return nullptr;
            }

            auto slot = first_free;
            first_free = first_free->next;
            slot->free = 0;
            ++object_count;
            return &slot->data;
        }

        void deallocate(pointer p)
        {
            if (p)
            {
                auto slot = slot_t::get_slot(p);
                slot->free = 1;
                slot->next = first_free;
                first_free = slot;
                --object_count;
            }
        }

        template<typename... Args>
        pointer construct(pointer p, Args&&... args)
        {
            new (p) value_type (std::forward<Args>(args)...);
        }

        void deconstruct(pointer p)
        {
            p->~value_type();
        }

        template<typename... Args>
        pointer make(Args&&... args)
        {
            pointer result = allocate();
            construct(std::forward<Args>(args)...);
            return result;
        }

        void destory(pointer p)
        {
            if (p)
            {
                deconstruct(p);
                deallocate(p);
            }
        }

        // will clear all slots and blocks and will unallocate then again.
        // this will create the same state as when it was first constructed
        // with nothing allocated
        void cleanup()
        {
            auto block = first_block;
            while (block != nullptr)
            {
                for (size_type i = 0 ; i < block->max ; ++i)
                {
                    if (block->data[i].free)
                        deconstruct(&block->data[i].data);
                }

                // move 'block' to the next one and return the buffer
                auto next = block->next;
                std::return_temporary_buffer(reinterpret_cast<uint8_t*>(block));
                block = next;
            }

            object_count = 0;
            block_count = 0;
            first_block = nullptr;
            first_free = nullptr;
        }

        size_type get_block_count()
        {
            return block_count;
        }

        size_type get_object_count()
        {
            return object_count;
        }

    private:
        bool allocate_block()
        {
            // // allocate a new block
            // auto info = std::get_temporary_buffer<uint8_t>(sizeof(block_t) + ObjectsPerBlock * sizeof(slot_t));
            // block_t* block = reinterpret_cast<block_t*>(info.first);

            // if (!block || info.second == 0)
            //     return false;

            // block->max = (info.second - sizeof(block_t)) / sizeof(slot_t);
            // block->next = first_block;
            // block->prev = nullptr;

            // if (!block->max)
            // {
            //     std::return_temporary_buffer(reinterpret_cast<uint8_t*>(block));
            //     return false;
            // }

            // for (size_type i = 0 ; i < block->max ; ++i)
            // {
            //     block->data[i].free = 1;
            //     block->data[i].next = i + 1 < block->max ?
            //         block->data + i + 1 :  first_free;
            // }

            // first_free = block->data;
            // first_block = block;
            // ++block_count;
            // return true;

            // // allocate a new block
            auto info = std::get_temporary_buffer<uint8_t>(sizeof(block_t) + ObjectsPerBlock * sizeof(slot_t));
            block_t *block = reinterpret_cast<block_t *>(info.first);

            if (!block || info.second == 0)
                return false;

            block->max = (info.second - sizeof(block_t)) / sizeof(slot_t);
            block->next = first_block;
            block->prev = nullptr;

            if (!block->max)
            {
                std::return_temporary_buffer(reinterpret_cast<uint8_t *>(block));
                return false;
            }

            for (size_t i = 0; i < block->max; ++i)
            {
                block->data[i].free = 1;
                block->data[i].next = i + 1 < block->max ?
                    block->data + i + 1 :  first_free;
            }

            first_free = block->data;
            first_block = block;
            ++block_count;
            return true;
        }

        size_type object_count;
        size_type block_count;
        block_t* first_block = nullptr;
        slot_t* first_free = nullptr;
    };
}}
