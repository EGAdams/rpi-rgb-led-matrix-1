/*  test_setbox.cpp  – interactive up/down positioning  */
#include <iostream>
#include <fstream>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/led-matrix.h"
#include "../include/graphics.h"

/* ---------------- tiny mock GameState ---------------- */
class MockGameState {
public:
    MockGameState() : _a(2), _b(1) {}
    int getTeamASets() const { return _a; }
    int getTeamBSets() const { return _b; }
private:
    int _a, _b;
};

/* ---------------- test drawer ------------------------ */
class TestSetDrawer {
public:
    TestSetDrawer(rgb_matrix::RGBMatrix* canvas,
                  MockGameState* gs)
        : _c(canvas), _gs(gs), _voffset(0)
    {
        if (!_font.LoadFont("fonts/the_sets_font.bdf") &&
            !_font.LoadFont("../fonts/the_sets_font.bdf")) {
            std::cerr << "Unable to load font.\n";
            exit(1);
        }
    }
    void setVerticalOffset(int v) { _voffset = v; }
    void draw()
    {
        _calcBox();
        _drawNumbers();
        _drawBorders();
        _drawDivider();
    }

private:
    /* position / size */
    static constexpr int START_ROW  = 89;
    static constexpr int PADDING    = 2;
    static constexpr int BOX_W      = 32;

    rgb_matrix::RGBMatrix* _c;
    MockGameState*         _gs;
    rgb_matrix::Font       _font;

    int _voffset;              // set from main loop
    int _x, _y, _w, _h;

    void _calcBox()
    {
        _w = BOX_W;
        _h = _font.height() + (PADDING * 2);

        _x = (_c->width() - _w) / 2;
        _y = START_ROW - PADDING - 10 + _voffset;   // <-- shift with offset
        if (_y < 0) _y = 0;
        if (_y + _h >= _c->height()) _y = _c->height() - _h;
    }
    void _drawBorders()
    {
        const rgb_matrix::Color blue(0,0,255);
        int r = _x + _w - 1, b = _y + _h - 1;
        for (int x=_x; x<=r; ++x) {                 // top & bottom
            _c->SetPixel(x,_y,blue.r,blue.g,blue.b);
            _c->SetPixel(x,b ,blue.r,blue.g,blue.b);
        }
        for (int y=_y; y<=b; ++y) {                 // left & right
            _c->SetPixel(_x,y,blue.r,blue.g,blue.b);
            _c->SetPixel(r ,y,blue.r,blue.g,blue.b);
        }
    }
    void _drawDivider()
    {
        const rgb_matrix::Color blue(0,0,255);
        int mid = _x + (_w/2) - 1;
        for (int y=_y; y<_y+_h; ++y) {
            _c->SetPixel(mid    ,y,blue.r,blue.g,blue.b);
            _c->SetPixel(mid + 1,y,blue.r,blue.g,blue.b);
        }
    }
    void _drawNumbers()
    {
        const std::string L = std::to_string(_gs->getTeamASets());
        const std::string R = std::to_string(_gs->getTeamBSets());
        const rgb_matrix::Color green(0,255,0), red(255,0,0);

        int half = _w/2;
        int lx = _x + (half - _strW(L))/2;
        int rx = _x + half + (half - _strW(R))/2;
        int ty = _y + _font.baseline() + 2;

        rgb_matrix::DrawText(_c,_font,lx,ty,green,nullptr,L.c_str());
        rgb_matrix::DrawText(_c,_font,rx,ty,red  ,nullptr,R.c_str());
    }
    int _strW(const std::string& s) const {
        int w{0}; for(char c:s){
            int cw=_font.CharacterWidth(uint32_t(c)); if(cw>0) w+=cw;}
        return w;
    }
};

/* ---------------- helpers: terminal raw mode ---------- */
class RawTerm {
public:
    RawTerm()
    {
        tcgetattr(STDIN_FILENO,&orig);
        termios raw = orig;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO,TCSANOW,&raw);
        /* make stdin non-blocking */
        int fl = fcntl(STDIN_FILENO,F_GETFL,0);
        fcntl(STDIN_FILENO,F_SETFL, fl | O_NONBLOCK);
    }
    ~RawTerm() { tcsetattr(STDIN_FILENO,TCSANOW,&orig); }
private:
    termios orig{};
};

/* ---------------- main -------------------------------- */
int main()
{
    rgb_matrix::RGBMatrix::Options o;
    o.hardware_mapping="regular";
    o.pixel_mapper_config="Rotate:180;264-Mapper";
    o.rows=64; o.cols=64; o.parallel=2; o.chain_length=1;
    o.disable_hardware_pulsing=true; o.brightness=35;
    o.pwm_bits=11; o.multiplexing=1;
    rgb_matrix::RuntimeOptions rt; rt.gpio_slowdown=2;

    std::unique_ptr<rgb_matrix::RGBMatrix> m(
        rgb_matrix::RGBMatrix::CreateFromOptions(o,rt));
    if(!m){ std::cerr<<"Matrix init failed\n"; return 1; }

    MockGameState gs;
    TestSetDrawer drawer(m.get(),&gs);
    RawTerm _raw;                                 // enter raw mode

    int v_off = 0;
    while(true){
        m->Clear();
        drawer.setVerticalOffset(v_off);
        drawer.draw();

        /* poll stdin */
        char buf[3];
        int n = read(STDIN_FILENO,buf,sizeof(buf));
        if(n<=0){ usleep(30'000); continue; }     // ~30 ms frame
        if(buf[0]=='q') break;                    // quit

        if(buf[0]==0x1B && n==3 && buf[1]=='['){  // ESC [ A/B
            if(buf[2]=='A') v_off--;              // up arrow
            if(buf[2]=='B') v_off++;              // down arrow
        }
    }

    m->Clear();
    return 0;
}
