#include <pride/pride.hpp>

int main()
{
    using namespace pride::console;
    using namespace std;

    cout << "Hi welcome to pride! This is the normal message color and style\n";
    cout << fg_t::blue << "Why are you feeling blue? We have colors that we can use!\n\n";
    
    cout << fg_t::reset << "Ok so what colors can we use? Lets list some colors\n";
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << "\n\n";

    cout << "Ok that is a lot of colors, what other colors can you use? What about the background?\n";
    cout << bg_t::black << "Black, " << bg_t::blue << "Blue, " << bg_t::cyan << "Cyan, "
         << bg_t::green << "Green, " << bg_t::grey << "Grey, " << bg_t::magenta << "Magenta, "
         << bg_t::red << "Red, " << bg_t::yellow << "Yellow." << bg_t::reset << "\n\n";

    cout << "Awesome we can change the forgound and background colors. Now what about the style?\n";
    cout << "Blink:\n" << style_t::blink;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Bold:\n" << style_t::bold;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Conceal:\n" << style_t::conceal;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Crossed:\n" << style_t::crossed;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Dim:\n" << style_t::dim;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Italic:\n" << style_t::italic;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Rblink:\n" << style_t::rblink;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Reversed:\n" << style_t::reversed;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";
    cout << "Underline:\n" << style_t::underline;
    cout << fg_t::black << "Black, " << fg_t::blue << "Blue, " << fg_t::cyan << "Cyan, "
         << fg_t::green << "Green, " << fg_t::grey << "Grey, " << fg_t::magenta << "Magenta, "
         << fg_t::red << "Red, " << fg_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n\n";

    cout << "Wow that is very stylish! We can also make the fg && bg bright/bold:\n";
    cout << fgb_t::black << "Black, " << fgb_t::blue << "Blue, " << fgb_t::cyan << "Cyan, "
         << fgb_t::green << "Green, " << fgb_t::grey << "Grey, " << fgb_t::magenta << "Magenta, "
         << fgb_t::red << "Red, " << fgb_t::yellow << "Yellow." << fg_t::reset << style_t::reset << "\n";

    cout << bgb_t::black << "Black, " << bgb_t::blue << "Blue, " << bgb_t::cyan << "Cyan, "
         << bgb_t::green << "Green, " << bgb_t::grey << "Grey, " << bgb_t::magenta << "Magenta, "
         << bgb_t::red << "Red, " << bgb_t::yellow << "Yellow." << bg_t::reset << style_t::reset << "\n\n";
}
