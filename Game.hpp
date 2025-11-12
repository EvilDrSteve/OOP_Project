
class Game {

    public:

        Game();
        ~Game();
        
        void init(const char* windowTitle, int width, int height, bool fullScreen);
        void update();
        void render();
        void handleEvents();

    private:
        bool isRunning;
};