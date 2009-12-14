#include "common.h"
#include "game.h"

BITMAP* buffer;
void swap( void )
{
    stretch_blit(buffer, screen, 0,0,WIDTH,HEIGHT, 0,0,800,480);
}

volatile int frames;
static void framec( void )
{
    frames++;
}

void error(const char* str)
{
    fprintf(stderr, "%s\n", str);
}

static int load_resources( void )
{
    Resource.tiles = load_pcx("data/tiles.pcx", 0);
    Resource.mage = load_pcx("data/mage.pcx", 0);
    if (Resource.tiles == NULL) {
        error("Couldn't load tiles.pcx");
        return -1;
    }
    if (Resource.mage == NULL) {
        error("Couldn't load mage.pcx");
        return -2;
    }
    return 0;
}

static int destroy_resources( void )
{
    if (Resource.tiles != NULL) {
        destroy_bitmap(Resource.tiles);
    }
    if (Resource.mage != NULL) {
        destroy_bitmap(Resource.mage);
    }
    return 0;
}


static int menu()
{
    game_start();
    return 0;
}

static int init()
{
    allegro_init();
    install_keyboard();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_NONE, 0);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 480, 0, 0);
    buffer = create_bitmap(WIDTH,HEIGHT);
    clear(buffer);
    load_resources();

    install_int_ex(framec, BPS_TO_TIMER(FRAMES_PER_SECOND));
    return 0;
}

static int quit()
{
    remove_int(framec);

    destroy_resources();
    destroy_bitmap(buffer);
    allegro_exit();
    return 0;
}

int main(int argc, char** argv)
{
    init();
    menu();
    quit();
    return 0;
}
END_OF_MAIN()

