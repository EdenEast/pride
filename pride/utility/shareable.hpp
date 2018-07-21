
#pragma once

#include <memory>

namespace pride
{
    template<typename Derived>
    class shareable : public std::enable_shared_from_this<Derived>
    {
        using std::enable_shared_from_this<Derived>::shared_from_this;
        friend Derived;

    public:
        using ptr = std::shared_ptr<Derived>;
        using const_ptr = std::shared_ptr<const Derived>;
        using weak_ptr = std::weak_ptr<Derived>;
        using const_weak_ptr = std::weak_ptr<const Derived>;

        ptr get() { return this->share_from_this(); }
        const_ptr get() const { return this->shared_from_this(); }

        weak_ptr get_weak() { return this->weak_from_this(); }
        const_weak_ptr get_weak() const { return this->weak_from_this(); }

        template<typename... Args>
        static ptr make_new(Args&&... args)
        {
            return ptr(new Derived(std::forward<Args>(args)...));
        }

    protected:
        // using allocator = std::_Ref_count_obj<Derived>;
        // using allocator = std::_Ref_count_obj<Derived>;
    };
}
