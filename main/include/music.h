#ifndef MUSIC_H
#define MUSIC_H

typedef struct {
  uint32_t hertz;
  uint32_t ms;
} note;

const note bell1[] = {
	{329, 1000}, {261, 1000}, {0, 0}
};

const note bell2[] = {
	{391, 500}, {329, 500}, {261, 500}, {391, 500}, {329, 500}, {261, 1000}, {0, 0}
};

const note westminster[] = {
	{329, 500}, {415, 500}, {369, 500}, {246, 1000}, {329, 500},
	{369, 500}, {415, 500}, {329, 1000}, {0, 0}
};

const note* music_lookup[] = { bell1, bell2, westminster };
const size_t music_size = sizeof(music_lookup) / sizeof(music_lookup[0]);

#endif /* MUSIC_H */

