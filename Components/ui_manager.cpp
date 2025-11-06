#include "ui_manager.hpp"
#include "../neural_net/helper.hpp"


UIManager::UIManager(sf::RenderWindow& win, nn::NeuralNet& netRef)
  : window(win), net(netRef),
    grid(28, 28),
    histogram({500, 250}, 10),
    view(window.getDefaultView())
{

    grid.setSize(view.getSize().y / 2);
    float centerX = (view.getSize().x - grid.getSize()) * 0.5F;
    float centerY = (view.getSize().y - grid.getSize()) * 0.5F;
    grid.setPosition({centerX, centerY});

    histogram.setFontFromFile("assets/ZedMonoNerdFont-Regular.ttf");
    histogram.setBarLabels({"0","1","2","3","4","5","6","7","8","9"});
    histogram.setLabelColor(labelColor);
    histogram.setBarColor(barColor);
    histogram.setPosition({960.0F, 540.0F});
}


void UIManager::handleResize(const sf::Event& event) {
    // shit code should split up
    view = window.getView();
    if (const auto* resize = event.getIf<sf::Event::Resized>()) {
        view.setSize({static_cast<float>(resize->size.x),
                      static_cast<float>(resize->size.y)});
        view.setCenter(view.getSize() / 2.0F);
        window.setView(view);

        float viewX = view.getSize().x;
        float viewY = view.getSize().y;

        float margin = viewX * 0.05F;
        float marginTotal = 3.0F * margin;
        float availableWidth = viewX - marginTotal;

        float gridWidth = 0.55F * availableWidth;
        grid.setSize(gridWidth);
        float gridY =  0.5F * (viewY - grid.getSize());
        grid.setPosition({margin, gridY});

        float histogramWidth = availableWidth - gridWidth;
        float histogramHeight = 0.3 * viewY;
        float histogramX = margin + gridWidth + margin;
        float histogramY = 0.5F * (viewY - histogramHeight);
        histogram.setSize({histogramWidth, histogramHeight});
        histogram.setPosition({histogramX, histogramY});
    }
}


void UIManager::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        window.close();
        return;
    }

    handleResize(event);
    grid.updateStatus(window, event);
}


void UIManager::update() {
    const auto& image = nn::normalizeImage(grid.getImage());
    auto dist = net.classify(image);
    histogram.updateBars(dist);
}


void UIManager::draw() {
    window.clear(bg);
    window.draw(grid);
    window.draw(histogram);
    window.display();
}
