#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

extern int game_startup();
extern void game_update();
extern void game_shutdown();

// special note: you MUST make sure that you
// use the debug SFML libraries in your Debug
// build, and link to the release versions in your
// Release build!
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")

int width = 640;
int height = 480;
bool windowed = true;
const sf::String title = "My win32 gameloop - ESC to exit, F1 for fullscreen/window";


int main()
{
    sf::RenderWindow hWnd(sf::VideoMode(width, height), title);
    
    hWnd.setFramerateLimit(60);

    if (game_startup() < 0)
        return -1;

    while (hWnd.isOpen())
    {
        sf::Event event;
        while (hWnd.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                hWnd.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    hWnd.close();
                } 
                else if (event.key.code == sf::Keyboard::F1) {
                    windowed = !windowed;
                    hWnd.create(sf::VideoMode(width, height), 
                        title, 
                        (windowed ? sf::Style::Resize | sf::Style::Close : sf::Style::Fullscreen));
                }

                break;
            default:
                break;

            }
        }

        hWnd.clear(sf::Color(100, 149, 237, 255));

        game_update();

        hWnd.display();
    }

    game_shutdown();

    return 0;
}
