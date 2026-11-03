#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define LARGURA 800
#define ALTURA  600
#define FPS     120.0

int main(void)
{
    /* 1. Inicializacao da biblioteca e dos modulos */
    if (!al_init()) {
        fprintf(stderr, "Falha ao iniciar o Allegro.\n");
        return 1;
    }
    if (!al_install_keyboard() || !al_init_primitives_addon() || !al_install_mouse() || !al_init_font_addon()) {
        fprintf(stderr, "Falha ao iniciar teclado, mouse ou add-ons.\n");
        return 1;
    }

    /* 2. Criacao dos recursos */
    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    ALLEGRO_FONT* fonte = al_create_builtin_font();
    if (!janela || !timer || !fila || !fonte) {
        fprintf(stderr, "Falha ao criar janela, timer, fila ou fonte.\n");
        return 1;
    }
    al_set_window_title(janela, "Meu primeiro jogo em Allegro");

    /* 3. A fila recebe eventos da janela, do timer e do teclado */
    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());

    /* 4. Estado do jogo */
    float x = LARGURA / 2.0f, y = ALTURA / 2.0f;
    const float TAM = 40.0f, VEL = 4.0f;
    bool teclas[ALLEGRO_KEY_MAX] = { false };
    bool rodando = true, redesenhar = true;
   
    /* 5. Laco principal */
    al_start_timer(timer);
    while (rodando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila, &ev);

        switch (ev.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            rodando = false;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            teclas[ev.keyboard.keycode] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = false;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            teclas[ev.keyboard.keycode] = false;
            break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			x = ev.mouse.x;
			y = ev.mouse.y;
			redesenhar = true;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if (ev.mouse.button & 1) { // Botão esquerdo
				printf("Botão esquerdo do mouse pressionado em (%d, %d)\n", ev.mouse.x, ev.mouse.y);
			}
        case ALLEGRO_EVENT_TIMER:          /* logica: 60 vezes por segundo */
            if (teclas[ALLEGRO_KEY_LEFT])  x -= VEL;
            if (teclas[ALLEGRO_KEY_RIGHT]) x += VEL;
            if (teclas[ALLEGRO_KEY_UP])    y -= VEL;
            if (teclas[ALLEGRO_KEY_DOWN])  y += VEL;
            if (x < TAM / 2) x = TAM / 2;
            if (x > LARGURA - TAM / 2) x = LARGURA - TAM / 2;
            if (y < TAM / 2) y = TAM / 2;
            if (y > ALTURA - TAM / 2) y = ALTURA - TAM / 2;
            redesenhar = true;
            break;
        }

        /* 6. Desenho: so quando a fila esvazia, para nao acumular atraso */
        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(x - TAM / 2, y - TAM / 2, x + TAM / 2, y + TAM / 2,
                al_map_rgb(255, 140, 0));
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 10, 10, 0,
                "Setas movem o quadrado. ESC para sair.");
            al_flip_display();
        }
    }

    /* 7. Liberacao dos recursos, na ordem inversa da criacao */
    al_destroy_font(fonte);
    al_destroy_event_queue(fila);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    return 0;
}
    
    
    