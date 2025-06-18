#include "../include/pongis.h"
#include "../include/pongislib.h"
#include "../include/mario.h"
#include "../include/victory.h"

static void update_position(uint8_t key);
static void handle_collision(uint8_t player_id, uint32_t prev_x, uint32_t prev_y, uint8_t key);
static void update_screen();
static void remove_object(uint8_t id);
static void draw_object(uint8_t id);
static void print_scores();
static uint8_t collides(uint8_t obj1_id, uint8_t obj2_id);
static void set_start();
static void set_exit();
static void set_level ();
static void play_victory();

static uint64_t numPlayers;
static uint8_t firstSound = 0;
static uint8_t level = 0;
static uint8_t exit = 0;
static uint32_t screenWidth = 0;
static uint32_t screenHeight = 0;
int ballDx = 0, ballDy = 0;
static uint8_t last_pusher = P1_ID;


typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t radius;
    uint32_t color;
    uint32_t points;
} circle;


typedef struct {
    uint32_t x;
    uint32_t y;
} init_positions;


static void play_mario(){
    for (uint64_t i = 0; i < CANT_NOTAS_MARIO ; i++) {
        _syscall(SYS_BEEP, mario_frecuencias[i], 0, mario_duraciones[i]);
    }
}

static void play_victory() {

    for (uint64_t i = 0; i < CANT_VICTORY_NOTAS; i++) {
        make_sound(victoryFrecuencias[i], victoryDuraciones[i]);
    }
}

static const init_positions levelPositions[CANT_LEVELS][CANT_OBJECTS] = {
    {{50, 100}, {200, 200}, {150, 150}, {700, 700}},
    {{80, 100},{700, 600}, {300, 300},{900, 500}},
    {{120, 150}, {650, 400}, {350, 250}, {800, 550}},
    {{200, 300}, {600, 150}, {400, 400}, {850, 650}},
    {{150, 450}, {750, 300}, {450, 200}, {950, 550}},
    {{250, 200}, {550, 550}, {350, 350}, {700, 450}},
    {{300, 100}, {500, 600}, {400, 300}, {850, 250}},
    {{100, 400}, {650, 200}, {300, 500}, {900, 650}},
    {{180, 250}, {720, 350}, {450, 450}, {800, 150}}
} ;

static circle objects[CANT_OBJECTS] = {
    {0, 0, 20, 0x00FF0000, 0}, // Player 1
    {0, 0, 20, 0x000000FF, 0}, // Player 2
    {0, 0, 10, 0x00FFFFFF, 0}, // Ball
    {0, 0, 15, 0x00000000, 0} // Hole
};

static void set_level () {
    if (level == CANT_LEVELS) {
        print_scores();
        print_text(((screenWidth - CHAR_WIDTH * EXIT_TEXT_LENGTH) / 2), 10, EXIT_TEXT, PONGIS_BACKGROUND);
        if (objects[P1_ID].points > objects[P2_ID].points) {
            print_text(((screenWidth - CHAR_WIDTH * WIN_TEXT_LENGTH) / 2), (screenHeight - CHAR_HEIGHT) / 2, P1_WIN_TEXT, 0x00000000);
        } else {
            print_text(((screenWidth - CHAR_WIDTH * WIN_TEXT_LENGTH) / 2), (screenHeight - CHAR_HEIGHT) / 2, P2_WIN_TEXT, 0x00000000);
        }
        play_mario();
        set_exit();
        return;
    }
    for (uint64_t i = 0; i < CANT_OBJECTS; i++) {
        remove_object(i);
        objects[i].x = levelPositions[level][i].x;
        objects[i].y = levelPositions[level][i].y;
        if ( i != P2_ID || (numPlayers == 2)) {
            draw_object(i);
        }
    }
    level++;
}

static void set_exit() {
    objects[P1_ID].points = 0;
    objects[P2_ID].points = 0;

    firstSound = 0;
    level = 0;
    enable_toggle();
    exit = 1;
}

static void set_start() {

    set_level();

    uint64_t aux = get_screen_size();
    screenWidth = (uint32_t)(aux >> 32);
    screenHeight = (uint32_t)(aux & 0xFFFFFFFF);

    paint_screen(PONGIS_BACKGROUND);
    draw_object(P1_ID);
    if (numPlayers == 2) {
        draw_object(P2_ID);
    }
    draw_object(BALL_ID);
    draw_object(HOLE_ID);


    disable_toggle();
    exit = 0;
}

static uint8_t collides(uint8_t obj1_id, uint8_t obj2_id) {
    circle c1 = objects[obj1_id];
    circle c2 = objects[obj2_id];

    int dx = (int)c1.x - (int)c2.x;
    int dy = (int)c1.y - (int)c2.y;
    uint32_t dis = dx * dx + dy * dy;
    uint32_t rads = c1.radius + c2.radius;

    return dis <= rads * rads;
}

static void print_scores() {
    uint8_t str1[TOTAL_TEXT] = "Player 1: ";
    str1[POINTS_TEXT] = objects[P1_ID].points + '0';
    str1[POINTS_TEXT + 1] = '\0';
    print_text(10, 10, str1, objects[P1_ID].color);
    if (numPlayers == 2) {
        uint8_t str2[TOTAL_TEXT] = "Player 2: ";
        str2[POINTS_TEXT] = objects[P2_ID].points + '0';
        str2[POINTS_TEXT + 1] = '\0';
        print_text(10, 34, str2, objects[P2_ID].color);
    }
    print_text(((screenWidth - CHAR_WIDTH * EXIT_TEXT_LENGTH) / 2), 10, EXIT_TEXT, 0x00000000);
}

static void remove_object(uint8_t id) {
    draw_circle(objects[id].x, objects[id].y, objects[id].radius, PONGIS_BACKGROUND);
}

static void draw_object(uint8_t id) {
    draw_circle(objects[id].x, objects[id].y, objects[id].radius, objects[id].color);
}

void pongis_main(uint64_t argc, uint8_t* argv[]) {

    if (argv[1][0] > '2' || argv[1][0] < '0') {
        return;
    }

    numPlayers = argv[1][0] - '0';

    set_start();

    while (!exit) {
        sleep(1000/60);
        update_screen();
    }
    paint_screen(0x00000000);
}

static void update_screen() {

    print_scores();
    if (!firstSound) {
        print_text(((screenWidth - CHAR_WIDTH * WELCOME_TEXT_LENGTH) / 2), (screenHeight - CHAR_HEIGHT) / 2, WELCOME_TEXT, 0x00000000);
        play_mario();
        firstSound = 1;
        print_text(((screenWidth - CHAR_WIDTH * WELCOME_TEXT_LENGTH) / 2), (screenHeight - CHAR_HEIGHT) / 2, WELCOME_TEXT, PONGIS_BACKGROUND);
    }

    uint8_t buffer[BUFFER_SIZE];
    get_keys(buffer);

    for (int i = 0; buffer[i] && i < BUFFER_SIZE; i++) {
        remove_object(BALL_ID);
        if (buffer[i] == 'w' || buffer[i] == 'a' || buffer[i] == 's' || buffer[i] == 'd') {
            remove_object(P1_ID);
            update_position(buffer[i]);
            draw_object(P1_ID);
        } else if (numPlayers == 2 && (buffer[i] == 'i' || buffer[i] == 'j' || buffer[i] == 'k' || buffer[i] == 'l')) {
            remove_object(P2_ID);
            update_position(buffer[i]);
            draw_object(P2_ID);
        } else if (buffer[i] == ' ') {
            set_exit();
            return;
        }
        if (collides(BALL_ID, HOLE_ID)) {
            objects[last_pusher].points++;
            objects[BALL_ID].x = objects[HOLE_ID].x;
            objects[BALL_ID].y = objects[HOLE_ID].y;
            draw_object(HOLE_ID);
            draw_object(BALL_ID);
            play_victory();
            set_level();
            return;
        }
        draw_object(HOLE_ID);
        draw_object(BALL_ID);
    }
}

static void handle_collision(uint8_t player_id, uint32_t prev_x, uint32_t prev_y, uint8_t key) {
    uint8_t other_player = (player_id == P1_ID) ? P2_ID : P1_ID;
    uint8_t ball_id = BALL_ID;

    if (numPlayers == 2 && collides(player_id, other_player)) {
        objects[player_id].x = prev_x;
        objects[player_id].y = prev_y;
        return;
    }

    if (collides(player_id, ball_id)) {
        int32_t dx = 0, dy = 0;
        uint32_t prev_ball_x = objects[ball_id].x;
        uint32_t prev_ball_y = objects[ball_id].y;
        switch (key) {
            case 'a': case 'j': dx = -((int32_t)screenWidth)/20; break;
            case 'd': case 'l': dx = ((int32_t)screenWidth)/20; break;
            case 'w': case 'i': dy = -((int32_t)screenHeight)/15; break;
            case 's': case 'k': dy = ((int32_t)screenHeight)/15; break;
        }
        uint32_t new_x = objects[ball_id].x + dx;
        uint32_t new_y = objects[ball_id].y + dy;

        uint8_t out_of_bounds = 0;
        uint32_t ball_min = objects[ball_id].radius + EDGE_OFFSET;
        uint32_t ball_max_x = screenWidth - ball_min;
        uint32_t ball_max_y = screenHeight - ball_min;

        if (new_x < ball_min || new_x > ball_max_x || new_y < ball_min || new_y > ball_max_y) {
            out_of_bounds = 1;
        }
        uint8_t collision_with_other = numPlayers == 2 && collides(ball_id, other_player);

        if (out_of_bounds || collision_with_other) {
            objects[ball_id].x = prev_ball_x;
            objects[ball_id].y = prev_ball_y;
            objects[player_id].x = prev_x;
            objects[player_id].y = prev_y;
        } else {
            objects[ball_id].x = new_x;
            objects[ball_id].y = new_y;
            last_pusher = player_id;
        }
    }
}

static void update_position(uint8_t key) {
    uint8_t player_id;
    switch (key) {
        case 'w': case 'a': case 's': case 'd':
            player_id = P1_ID;
        break;
        case 'i': case 'j': case 'k': case 'l':
            player_id = P2_ID;
        break;
        default:
            return;
    }

    uint32_t prev_x = objects[player_id].x;
    uint32_t prev_y = objects[player_id].y;

    int32_t dx = 0, dy = 0;
    switch (key) {
        case 'a': case 'j': dx = -((int32_t)screenWidth)/60; break;
        case 'd': case 'l': dx = ((int32_t)screenWidth)/60; break;
        case 'w': case 'i': dy = -((int32_t)screenHeight)/45; break;
        case 's': case 'k': dy = ((int32_t)screenHeight)/45; break;
    }

    int32_t new_x = (int32_t)objects[player_id].x + dx;
    int32_t new_y = (int32_t)objects[player_id].y + dy;

    uint32_t min_x = objects[player_id].radius;
    uint32_t max_x = screenWidth - objects[player_id].radius;
    uint32_t min_y = objects[player_id].radius;
    uint32_t max_y = screenHeight - objects[player_id].radius;

    if (new_x < (int32_t)min_x) new_x = min_x;
    if (new_x > (int32_t)max_x) new_x = max_x;
    if (new_y < (int32_t)min_y) new_y = min_y;
    if (new_y > (int32_t)max_y) new_y = max_y;

    objects[player_id].x = (uint32_t)new_x;
    objects[player_id].y = (uint32_t)new_y;

    handle_collision(player_id, prev_x, prev_y, key);
}