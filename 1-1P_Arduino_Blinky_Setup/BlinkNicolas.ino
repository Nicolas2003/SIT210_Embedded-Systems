void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void shortBlink()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}

void longBlink()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(3000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(3000);
}

void letterSpace()
{
    delay(3000);
}

void loop()
{
    // The name Nicolas

    // First letter: N
    longBlink();
    shortBlink();
    letterSpace();

    // Second letter: I
    shortBlink();
    shortBlink();
    letterSpace();

    // Third letter: C
    longBlink();
    shortBlink();
    longBlink();
    shortBlink();
    letterSpace();

    // Fourth letter: O
    longBlink();
    longBlink();
    longBlink();
    letterSpace();

    // Fifth letter: L
    shortBlink();
    longBlink();
    shortBlink();
    shortBlink();
    letterSpace();

    // Sixth letter: A
    shortBlink();
    longBlink();
    letterSpace();

    // Seventh letter: S
    shortBlink();
    shortBlink();
    shortBlink();

    Serial.println("Program finished executing loop");
}
