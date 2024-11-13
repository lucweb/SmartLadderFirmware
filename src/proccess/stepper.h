#include "Generic.h"

#if defined(ESP32)
struct TimerData
{
  int id;
  int pin;
  unsigned long total;
  Generic *instance;
};

esp_timer_handle_t T_H[10];
#endif

bool Generic::stepper(int resource, String values)
{
  int p = 0;
  char v1[6] = "";
  char v2[8] = "";
  char v3[3] = "";
  char v4 = '0';
  int vIndex = 0;

  for (size_t i = 0; i < values.length(); i++)
  {
    if (values[i] == '/')
    {
      p++;
      vIndex = 0;
    }
    else
    {
      if (p == 0)
        v1[vIndex++] = values[i];
      else if (p == 1)
        v2[vIndex++] = values[i];
      else if (p == 2)
        v3[vIndex++] = values[i];
      else if (p == 3)
      {
        v4 = values[i];
      }
    }
  }

  int speed = atoi(v1);
  long int totalSteps = atoi(v2);
  int c = atoi(v3);

  if (v4 == '0')
    return basicStepper(resource, speed, totalSteps, c);
  // return true;
  return asyncStepper(resource, speed, totalSteps, c);
}

bool Generic::asyncStepper(int pin, int v, unsigned long totalSteps, int c)
{
  if (CT_CT[c] >= totalSteps)
    return true;
  for (size_t x = 0; x < totalSteps; x++)
  {
    digitalWrite(pin, 1);
    delayMicroseconds(v);
    digitalWrite(pin, 0);
    delayMicroseconds(v);
    CT_CT[c]++;
  }
  return true;
}

bool Generic::basicStepper(int pin, int v, unsigned long totalSteps, int c)
{
  if (CT_CT[c] >= totalSteps)
    return true;

#if defined(ESP32)
  if (C_CCCTU[c] < 1 || !esp_timer_is_active(T_H[C_CCCTU[c] - 1]))
    timerConfigCallback(pin, v, c, totalSteps);
#else
  if (!CT_CT[c])
    timerConfigCallback(pin, v, c, totalSteps);
#endif

  return false;
}

#if defined(ESP32)
void timerStepperCallback(void *arg)
{
  TimerData *data = (TimerData *)arg;
  int i = data->id;
  int p = data->pin;
  long int t = data->total;

  if (data->instance->CT_CT[i] >= t)
  {
    esp_timer_stop(T_H[data->instance->C_CCCTU[i] - 1]);
    digitalWrite(p, LOW);
    return;
  }

  digitalWrite(p, static_cast<int>(data->instance->CT_CT[i]) & 1);
  data->instance->CT_CT[i]++;
}

void Generic::timerConfigCallback(int pin, int v, int c, unsigned long totalSteps)
{
  TimerData *timer_data = new TimerData();
  timer_data->id = c;
  timer_data->pin = pin;
  timer_data->total = totalSteps;
  timer_data->instance = this;

  C_CCCTU[c] = 0;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  for (size_t i = 0; i < 10; i++)
  {
    if (!esp_timer_is_active(T_H[i]))
    {
      C_CCCTU[c] = i + 1;
      break;
    }
  }

  const esp_timer_create_args_t timer_args = {
      .callback = &timerStepperCallback,
      .arg = timer_data,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "timer",
      .skip_unhandled_events = false};

  esp_timer_create(&timer_args, &T_H[C_CCCTU[c] - 1]);
  esp_timer_start_periodic(T_H[C_CCCTU[c] - 1], v);
}

#else

struct StpTmrDt
{
  int id;
  unsigned long total;
  int speed;
  int msSpeed;
};

Generic *instance;
StpTmrDt STP[4];
bool isrTime = false;

void setISR(int i)
{
  auto &stepData = STP[i];
  stepData.msSpeed++;
  if (stepData.msSpeed >= stepData.speed)
  {
    int x = stepData.id - 1;
    stepData.msSpeed = 0;
    if (instance->CT_CT[x] >= stepData.total)
    {
      digitalWrite(instance->C_CCCTU[x], LOW);
      stepData.id = 0;
    }
    else
    {
      digitalWrite(instance->C_CCCTU[x], static_cast<int>(instance->CT_CT[x]) & 1);
      instance->CT_CT[x]++;
    }
  }
}

#if defined(__AVR_ATmega2560__)
void startISR()
{
  isrTime = true;
  cli();
  TCCR3A = 0;
  TCCR3B = 0;
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS31);
  TIMSK3 |= (1 << OCIE3A);
  OCR3A = 99;
  sei();
}

ISR(TIMER3_COMPA_vect)
{
  bool stopTimer = true;
  if (STP[0].id)
  {
    setISR(0);
    stopTimer = false;
  }

  if (STP[1].id)
  {
    setISR(1);
    stopTimer = false;
  }

  if (STP[2].id)
  {
    setISR(2);
    stopTimer = false;
  }

  if (STP[3].id)
  {
    setISR(3);
    stopTimer = false;
  }

  if (stopTimer)
    instance->stpTimer();
}

void Generic::stpTimer()
{
  TIMSK3 &= ~(1 << OCIE3A);
  TCCR3B &= 0xF8;
  OCR3A = 0;
}

void Generic::rstTimer()
{
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS31); // CS30
  TIMSK3 |= (1 << OCIE3A);
  OCR3A = 99; // 159;
}

#else
void startISR()
{
  isrTime = true;
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= (1 << WGM12);
  OCR2A = 159;
  TCCR2B |= (1 << CS20);
  TIMSK2 |= (1 << OCIE2A);
  sei();
}

ISR(TIMER2_COMPA_vect)
{
  bool stopTimer = true;
  if (STP[0].id)
  {
    setISR(0);
    stopTimer = false;
  }

  if (STP[1].id)
  {
    setISR(1);
    stopTimer = false;
  }

  if (STP[2].id)
  {
    setISR(2);
    stopTimer = false;
  }

  if (STP[3].id)
  {
    setISR(3);
    stopTimer = false;
  }

  if (stopTimer)
    instance->stpTimer();
}

void Generic::stpTimer()
{
  TIMSK2 &= ~(1 << OCIE2A);
  TCCR2B &= 0xF8;
  OCR2A = 0;
}

void Generic::rstTimer()
{
  TCCR2B |= (1 << CS20);
  TIMSK2 |= (1 << OCIE2A);
  OCR1A = 159;
}
#endif

void Generic::timerConfigCallback(int pin, int v, int c, unsigned long steeps)
{
  C_CCCTU[c] = pin;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  for (size_t i = 0; i < 4; i++)
  {
    if (!STP[i].id)
    {
      STP[i].id = c + 1;
      STP[i].total = steeps;
#if defined(__AVR_ATmega2560__)
      STP[i].speed = v / 50;
#else
      STP[i].speed = v / 10;
#endif
      STP[i].msSpeed = 0;
      break;
    }
  }

  if (!isrTime)
  {
    instance = this;
    startISR();
  }
  else
    rstTimer();
}

#endif