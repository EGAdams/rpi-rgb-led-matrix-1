#ifndef TESTDISPLAY_H
#define TESTDISPLAY_H

#include "../IDisplay/IDisplay.h"
#include <string>

class TestDisplay : public IDisplay {
public:
    TestDisplay();
    virtual ~TestDisplay();

    void setColor(const std::string& colorName) override;
    void drawText(const std::string& text, int x, int y) override;
    void clearScreen() override;
};

#endif // TESTDISPLAY_H