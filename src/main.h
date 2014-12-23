#define SECONDS_IN_MINUTE 60;
#define WARMUP_DEFAULT 20
#define MEDITATE_DEFAULT 20 * SECONDS_IN_MINUTE;
#define COOLDOWN_DEFAULT 3 * SECONDS_IN_MINUTE;
#define WARMUP_PKEY 1
#define MEDITATE_PKEY 2
#define COOLDOWN_PKEY 3
void timer_change_handler();
void phase_change_handler();
void show_main(void);
void hide_main(void);
