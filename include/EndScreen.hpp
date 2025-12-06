#include <SFML/Graphics.hpp>

struct EndScreenData {
    int tablesServed = 0;
    int tablesLost   = 0;
    int levelScore   = 0;
};

class EndScreen {
public:
    EndScreen(sf::RenderWindow* window, sf::Font* font);
    ~EndScreen();

    void setData(const EndScreenData& data);
    void setVisible(bool visible);
    bool isVisible() const;

    void update(const sf::Vector2f& mousePos, bool mousePressed);
    void render();

    bool replayPressed  = false;
    bool mainMenuPressed = false;

private:
    sf::RenderWindow* window;
    sf::Font* font;
    EndScreenData data;
    bool visible;

    sf::RectangleShape overlay;

    sf::RectangleShape outerFrame;
    sf::RectangleShape innerPanel;

    sf::Text title;
    sf::Text servedLabel, lostLabel, servedValue, lostValue, scoreLabel, scoreValue;

    sf::RectangleShape hLineTop, hLineBottom, vLineMiddle;

    sf::Texture replayTex, mainMenuTex;
    sf::Sprite  replayBtn, mainMenuBtn;

    void build();
    void updateTexts();
    void layoutColumns(const sf::Vector2f& panelPos, const sf::Vector2f& panelSize);
};