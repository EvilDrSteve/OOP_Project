#include "EndScreen.hpp"
#include <cmath>
#include <stdexcept>

EndScreen::EndScreen(sf::RenderWindow* window, sf::Font* font)
    : window(window), font(font), visible(true)
{
    if (!window || !font) throw std::runtime_error("EndScreen: window/font is null");
    build();
}

EndScreen::~EndScreen() {}

void EndScreen::setData(const EndScreenData& d) {
    data = d;
    updateTexts();
}

void EndScreen::setVisible(bool v) { visible = v; }
bool EndScreen::isVisible() const { return visible; }

void EndScreen::update(const sf::Vector2f& mousePos, bool mousePressed) {
    if (!visible) return;

    if (mousePressed) {
        if (replayBtn.getGlobalBounds().contains(mousePos)) {
            if (onReplay) onReplay();
        }
        if (mainMenuBtn.getGlobalBounds().contains(mousePos)) {
            if (onMainMenu) onMainMenu();
        }
    }
}

void EndScreen::render() {
    if (!visible) return;

    window->draw(overlay);
    window->draw(outerFrame);
    window->draw(innerPanel);

    window->draw(title);
    window->draw(servedLabel);
    window->draw(lostLabel);
    window->draw(servedValue);
    window->draw(lostValue);
    window->draw(scoreLabel);
    window->draw(scoreValue);

    window->draw(hLineTop);
    window->draw(hLineBottom);
    window->draw(vLineMiddle);

    window->draw(replayBtn);
    window->draw(mainMenuBtn);
}


void EndScreen::build() {
    float W = window->getSize().x;
    float H = window->getSize().y;

    overlay.setSize({W, H});
    overlay.setFillColor(sf::Color(0, 0, 0, 140));

    sf::Vector2f panelSize(W * 0.60f, H * 0.78f);
    sf::Vector2f panelPos((W - panelSize.x) / 2.f, (H - panelSize.y) / 2.f - 20.f);

    outerFrame.setSize(panelSize + sf::Vector2f(20, 20));
    outerFrame.setPosition(panelPos - sf::Vector2f(10, 10));
    outerFrame.setFillColor(sf::Color(160, 120, 120));
    outerFrame.setOutlineColor(sf::Color(90, 60, 60));
    outerFrame.setOutlineThickness(3.f);

    innerPanel.setSize(panelSize);
    innerPanel.setPosition(panelPos);
    innerPanel.setFillColor(sf::Color(255, 255, 255));

    title.setFont(*font);

    if (data.tablesServed <= 3) {
        title.setString("Try Again");
        title.setFillColor(sf::Color::Red);
        title.setCharacterSize(36); 
    } else {
        title.setString("Thank You");
        title.setFillColor(sf::Color(30, 30, 160));
        title.setCharacterSize(42);
    }


    {
        sf::FloatRect b = title.getLocalBounds();
        title.setPosition(panelPos.x + (panelSize.x - b.width) / 2.f - b.left,
                          panelPos.y + 15.f);
    }

    float lineLeft  = panelPos.x + 20.f;
    float lineRight = panelPos.x + panelSize.x - 20.f;

    float topY = title.getGlobalBounds().top + title.getGlobalBounds().height + 15.f;

    hLineTop.setSize({lineRight - lineLeft, 2});
    hLineTop.setFillColor(sf::Color::Black);
    hLineTop.setPosition(lineLeft, topY);

    float midY = topY + panelSize.y * 0.30f;

    hLineBottom.setSize({lineRight - lineLeft, 2});
    hLineBottom.setFillColor(sf::Color::Black);
    hLineBottom.setPosition(lineLeft, midY);

    vLineMiddle.setSize({2, midY - topY});
    vLineMiddle.setFillColor(sf::Color::Black);
    vLineMiddle.setPosition(panelPos.x + panelSize.x / 2.f, topY);

    float leftColX  = panelPos.x + panelSize.x * 0.25f;
    float rightColX = panelPos.x + panelSize.x * 0.75f;
    float labelY    = topY + 10.f;

    auto center = [](sf::Text& t, float x, float y){
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition(x - (b.width/2.f + b.left), y);
    };

    servedLabel.setFont(*font);
    servedLabel.setCharacterSize(20);
    servedLabel.setFillColor(sf::Color::Black);
    servedLabel.setString("Tables\nserved");
    center(servedLabel, leftColX, labelY);

    servedValue.setFont(*font);
    servedValue.setCharacterSize(32);
    servedValue.setFillColor(sf::Color::Black);
    center(servedValue, leftColX, labelY + 55.f);

    lostLabel.setFont(*font);
    lostLabel.setCharacterSize(20);
    lostLabel.setFillColor(sf::Color::Black);
    lostLabel.setString("Tables\nlost");
    center(lostLabel, rightColX, labelY);

    lostValue.setFont(*font);
    lostValue.setCharacterSize(32);
    lostValue.setFillColor(sf::Color::Red);
    center(lostValue, rightColX, labelY + 55.f);

    float midX = panelPos.x + panelSize.x / 2.f;

    scoreLabel.setFont(*font);
    scoreLabel.setCharacterSize(22);
    scoreLabel.setFillColor(sf::Color::Black);
    scoreLabel.setString("Level Score");

    center(scoreLabel, midX, midY + 12.f);     

    scoreValue.setFont(*font);
    scoreValue.setCharacterSize(36);
    scoreValue.setFillColor(sf::Color(20, 160, 20));
    
    center(scoreValue, midX - 40.f, midY + 48.f);     

    replayTex.loadFromFile("assets/replay_button.png");
    mainMenuTex.loadFromFile("assets/mainmenu_button.png");

    replayBtn.setTexture(replayTex);
    mainMenuBtn.setTexture(mainMenuTex);

    float buttonW = 260.f;
    float buttonH = 70.f;

    replayBtn.setScale(
        buttonW / replayBtn.getLocalBounds().width,
        buttonH / replayBtn.getLocalBounds().height
    );
    mainMenuBtn.setScale(
        buttonW / mainMenuBtn.getLocalBounds().width,
        buttonH / mainMenuBtn.getLocalBounds().height
    );

    replayBtn.setOrigin(replayBtn.getLocalBounds().width/2, replayBtn.getLocalBounds().height/2);
    mainMenuBtn.setOrigin(mainMenuBtn.getLocalBounds().width/2, mainMenuBtn.getLocalBounds().height/2);

    replayBtn.setPosition(midX, midY + 130.f);
    mainMenuBtn.setPosition(midX - 10.f, midY + 200.f);   

    updateTexts();
}


void EndScreen::updateTexts() {
    servedValue.setString(std::to_string(data.tablesServed));
    lostValue.setString(std::to_string(data.tablesLost));
    scoreValue.setString(std::to_string(data.levelScore));

    // if (data.tablesServed <= 3) {
    //     title.setString("Try Again");
    // } else {
    //     title.setString("Thank You");
    // }
    if (data.tablesServed <= 3) {
        title.setString("Try Again");
        title.setFillColor(sf::Color::Red);
        title.setCharacterSize(36); 
    } else {
        title.setString("Thank You");
        title.setFillColor(sf::Color(30, 30, 160));
        title.setCharacterSize(42);
    }


    sf::FloatRect b = title.getLocalBounds();
    sf::Vector2f panelPos = innerPanel.getPosition();
    sf::Vector2f panelSize = innerPanel.getSize();
    title.setPosition(panelPos.x + (panelSize.x - b.width) / 2.f - b.left,
                      panelPos.y + 15.f);

}

void EndScreen::layoutColumns(const sf::Vector2f& panelPos, const sf::Vector2f& panelSize) {
    float leftColX   = panelPos.x + panelSize.x * 0.25f;
    float rightColX  = panelPos.x + panelSize.x * 0.75f;
    float colYTop    = hLineTop.getPosition().y + 10.f;
    float valY       = colYTop + 50.f;

    // Left column
    sf::FloatRect bL = servedLabel.getLocalBounds();
    servedLabel.setPosition(std::floor(leftColX - (bL.width / 2.f + bL.left)), std::floor(colYTop));

    sf::FloatRect bLV = servedValue.getLocalBounds();
    servedValue.setPosition(std::floor(leftColX - (bLV.width / 2.f + bLV.left)), std::floor(valY));

    // Right column
    sf::FloatRect bR = lostLabel.getLocalBounds();
    lostLabel.setPosition(std::floor(rightColX - (bR.width / 2.f + bR.left)), std::floor(colYTop));

    sf::FloatRect bRV = lostValue.getLocalBounds();
    lostValue.setPosition(std::floor(rightColX - (bRV.width / 2.f + bRV.left)), std::floor(valY));

    // Score row
    sf::FloatRect bSL = scoreLabel.getLocalBounds();
    scoreLabel.setPosition(
        std::floor(panelPos.x + (panelSize.x - bSL.width) / 2.f - bSL.left),
        std::floor(hLineBottom.getPosition().y - 30.f)
    );

    sf::FloatRect bSV = scoreValue.getLocalBounds();
    scoreValue.setPosition(
        std::floor(panelPos.x + (panelSize.x - bSV.width) / 2.f - bSV.left),
        std::floor(hLineBottom.getPosition().y + 10.f)
    );
}