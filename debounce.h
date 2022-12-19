/**
 * @file debounce.c
 * @author Sail128 (Roan van der Voort)
 * @brief a simple library to debounce an input
 * @version 0.1
 * @date 2022-12-18
 *
 * @copyright Copyright (c) 2022
 *
 * MIT License

Copyright (c) 2022 Roan van der Voort

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#ifndef DEBOUNCE_MILLIS
#define DEBOUNCE_MILLIS millis // function for getting the timer
#endif

enum debounce_state_e
{
  DEBOUNCE_BASE_STATE,
  DEBOUNCE_DELAY_STATE
};

struct debounce_t
{
  enum debounce_state_e state;
  long last_time;
  int output;
  int delay;
};

void debounce_init(struct debounce_t *signal, int delay);
int debounce(struct debounce_t *signal, int input);
int debounce_notimer(struct debounce_t *signal, int input);

#endif // DEBOUNCE_H

#ifdef DEBOUNCE_IMPL
void debounce_init(struct debounce_t *signal, int delay)
{
  signal->state = DEBOUNCE_BASE_STATE;
  signal->last_time = 0;
  signal->output = 0;
  signal->delay = delay;
}

int debounce(struct debounce_t *signal, int input)
{
  long current_time = DEBOUNCE_MILLIS();

  switch (signal->state)
  {
  case DEBOUNCE_BASE_STATE:
    if (input != signal->output)
    {
      signal->state = DEBOUNCE_DELAY_STATE;
      signal->last_time = current_time;
    }

    break;
  case DEBOUNCE_DELAY_STATE:
    if ((current_time - signal->last_time) > signal->delay)
    {
      signal->state = DEBOUNCE_BASE_STATE;
      signal->output = !signal->output;
    }
    else if (input == signal->output)
    {
      signal->state = DEBOUNCE_BASE_STATE;
    }
    break;
  default:
    break;
  }

  return signal->output;
}

/**
 * no timer requires a set number of calls with the new input state.
 * if delay is 5 it takes 5 calls with the new state to switch the output
 * create for when an input is checked on a schdule already.
 */
int debounce_notimer(struct debounce_t *signal, int input)
{
  // long current_time = DEBOUNCE_MILLIS();

  switch (signal->state)
  {
  case DEBOUNCE_BASE_STATE:
    if (input != signal->output)
    {
      signal->state = DEBOUNCE_DELAY_STATE;
      signal->last_time = 1;
    }

  case DEBOUNCE_DELAY_STATE:
    if (signal->last_time >= signal->delay)
    {
      signal->state = DEBOUNCE_BASE_STATE;
      signal->output = !signal->output;
    }
    else if (input == signal->output)
    {
      signal->state = DEBOUNCE_BASE_STATE;
    }
    else
    {
      signal->last_time++;
    }
    break;
  default:
    break;
  }

  return signal->output;
}
#endif // DEBOUNCE_IMPL