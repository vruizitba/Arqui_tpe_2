#define NOTE_E4  330
#define NOTE_E5  659
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_G4  196
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_AS4 466
#define NOTE_F5  698
#define NOTE_D5  587

uint16_t mario_frecuencias[] = {
  // Intro icónico
  NOTE_E5, NOTE_E5, 0, NOTE_E5,
  0, NOTE_C5, NOTE_E5, 0,
  NOTE_G5, 0, 0, 0,
  NOTE_G4, 0, 0, 0,

  NOTE_C5, 0, 0, NOTE_G4,
  0, 0, NOTE_E4, 0,
  0, NOTE_A4, 0, NOTE_B4,
  0, NOTE_AS4, NOTE_A4, 0,

  NOTE_G4, NOTE_E5, NOTE_G5,
  NOTE_A4, 0, NOTE_F5, NOTE_G5,
  0, NOTE_E5, 0, NOTE_C5,
  NOTE_D5, NOTE_B4, 0, 0
};

uint16_t mario_duraciones[] = {

  180, 180, 180, 180,
  180, 180, 180, 180,
  180, 180, 180, 180,
  180, 180, 180, 180,

  180, 180, 180, 180,
  180, 180, 180, 180,
  180, 180, 180, 180,
  180, 180, 180, 180,

  120, 180, 180, 180,
  180, 180, 180, 180,
  180, 180, 180, 180,
  180, 180, 180, 180
};