#pragma once

#include <vector>
#include <numeric>
#include <functional>

class ValueFilter {
private:
    std::vector<float> buffer;    // Ringpuffer für die Werte
    int bufferSize;               // Größe des Puffers
    int index;                    // Aktuelle Position im Puffer
    float adaptiveThreshold;      // Dynamischer Schwellenwert
    bool bufferFilled;            // Flag, um zu überprüfen, ob der Puffer gefüllt ist
    float alpha;                  // Glättungsfaktor für die exponentielle Glättung

public:
    ValueFilter(int size, float smoothingFactor = 0.3f) 
        : bufferSize(size), index(0), bufferFilled(false), adaptiveThreshold(0.0), alpha(smoothingFactor) {
        buffer.resize(size, 0.0);
    }

    // Berechne den gewichteten Mittelwert der Werte im Puffer (für den Fall der linearen Gewichtung)
    float calculateWeightedMean() {
        float weightedSum = 0.0;
        float weightSum = 0.0;

        // Ältere Werte erhalten geringere Gewichte
        for (int i = 0; i < bufferSize; ++i) {
            // Berechne das Gewicht: neuere Werte haben höhere Gewichtung
            float weight = bufferSize - i;  // Einfache Gewichtung: neuere Werte bekommen höhere Gewichtung
            weightedSum += buffer[i] * weight;
            weightSum += weight;
        }

        // Rückgabe des gewichteten Mittelwerts
        return weightedSum / weightSum;
    }

    // Berechne den geglätteten Wert mittels exponentieller Glättung
    float calculateExponentialMovingAverage() {
        float ema = buffer[0];  // Initialisiere mit dem ersten Wert im Puffer
        for (int i = 1; i < bufferSize; ++i) {
            ema = alpha * buffer[i] + (1 - alpha) * ema;  // Exponentielle Glättung
        }
        return ema;
    }

    // Berechne die Standardabweichung der Werte im Puffer
    float calculateStdDev(float mean) {
        float sumSqDiff = 0.0;
        for (float value : buffer) {
            sumSqDiff += std::pow(value - mean, 2);
        }
        return std::sqrt(sumSqDiff / bufferSize);
    }

    // Füge einen neuen Wert hinzu und berechne den gefilterten Wert
    float processValue(float newValue) {
        // Wenn der Puffer noch nicht voll ist, fülle ihn mit dem neuen Wert
        buffer[index] = newValue;
        index = (index + 1) % bufferSize;

        // Prüfe, ob der Puffer vollständig gefüllt ist
        if (index == 0) {
            bufferFilled = true;
        }

        // Berechne den Mittelwert und die Standardabweichung, wenn der Puffer gefüllt ist
        float mean = 0.0;
        float stddev = 0.0;
        if (bufferFilled) {
            mean = calculateExponentialMovingAverage();  // Berechne den geglätteten Mittelwert
            stddev = calculateStdDev(mean);
            
            // Berechne den adaptiven Schwellenwert (z.B. 2x die Standardabweichung)
            adaptiveThreshold = 2.0 * stddev;  // Schwellenwert als 2x die Standardabweichung
        }

        // Prüfe, ob der neue Wert ein Ausreißer ist (abweichend vom Mittelwert)
        if (bufferFilled && std::abs(newValue - mean) > adaptiveThreshold) {
            // Wert ist ein Ausreißer, verwende stattdessen den Mittelwert
            newValue = mean;
        }

        // Rückgabe des gefilterten Werts
        return newValue;
    }
};


class ThermalPowerCalculator {
private:
    float specificHeatCapacity; // Wärmekapazität (z. B. 4.18 kJ/kgK für Wasser)
    int delayWindow;            // Verzögerung (Anzahl der Zyklen)
    std::deque<float> bufferFlow;    // Puffer für Durchfluss (in L/h)
    std::deque<float> bufferTempIn;  // Puffer für Vorlauftemperatur (Tv)
    std::deque<float> bufferTempOut; // Puffer für Rücklauftemperatur (Tr)

    // Prüft retrospektiv auf Ausreißer und glättet sie
    void checkAndCorrectOutliers(std::deque<float>& buffer) {
        if (buffer.size() < 3) {
            return; // Nicht genügend Daten für retrospektive Prüfung
        }

        // Der vorletzte Wert im Puffer (aktueller Analysepunkt)
        float current = buffer[buffer.size() - 2];
        float previous = buffer[buffer.size() - 3];
        float next = buffer.back();

        // Ausreißer-Bedingung: Der Wert weicht stark von vorherigem und nächstem Wert ab
        if (std::abs(current - previous) > 2.0 && std::abs(current - next) > 2.0) {
            buffer[buffer.size() - 2] = previous; // Wert korrigieren
        }
    }

    // Füge einen neuen Wert hinzu und starte Ausreißerprüfung
    void addToBuffer(std::deque<float>& buffer, float value) {
        buffer.push_back(value);
        if (buffer.size() > delayWindow + 1) {
            checkAndCorrectOutliers(buffer);
        }
    }

public:
    ThermalPowerCalculator(float specificHeat, int delay)
        : specificHeatCapacity(specificHeat), delayWindow(delay) {}

    // Aktualisiere die Vorlauftemperatur (Tv)
    void updateTempIn(float tempIn) {
        addToBuffer(bufferTempIn, tempIn);
    }

    // Aktualisiere die Rücklauftemperatur (Tr)
    void updateTempOut(float tempOut) {
        addToBuffer(bufferTempOut, tempOut);
    }

    // Aktualisiere den Durchfluss (in L/h)
    void updateFlow(float flowLPerHour) {
        addToBuffer(bufferFlow, flowLPerHour);
    }

    // Berechne die thermische Leistung
    float calculatePower() {
        // Sicherstellen, dass mindestens 2 Werte vorhanden sind
        if (bufferFlow.size() < 2 || bufferTempIn.size() < 2 || bufferTempOut.size() < 2) {
            return 0.0; // Noch nicht genügend Daten
        }

        // Werte aus dem Puffer extrahieren (verzögerte Werte)
        float delayedFlow = bufferFlow[bufferFlow.size() - 2];
        float delayedTempIn = bufferTempIn[bufferTempIn.size() - 2];
        float delayedTempOut = bufferTempOut[bufferTempOut.size() - 2];

        // Umrechnung des Durchflusses von L/h in kg/s
        float flowKgPerSec = delayedFlow / 3600.0;

        // Temperaturdifferenz berechnen
        float deltaTemp = delayedTempIn - delayedTempOut;

        // Thermische Leistung berechnen
        return flowKgPerSec * specificHeatCapacity * deltaTemp; // Leistung in kW
    }
};



/*

class ValueFilter {
private:
    std::vector<float> buffer;    // Ringpuffer für die Werte
    int bufferSize;               // Größe des Puffers
    int index;                    // Aktuelle Position im Puffer
    float adaptiveThreshold;      // Dynamischer Schwellenwert
    bool bufferFilled;            // Flag, um zu überprüfen, ob der Puffer gefüllt ist

public:
    ValueFilter(int size) 
        : bufferSize(size), index(0), bufferFilled(false), adaptiveThreshold(0.0) {
        buffer.resize(size, 0.0);
    }

    // Berechne den Mittelwert der Werte im Puffer
    float calculateMean() {
        return std::accumulate(buffer.begin(), buffer.end(), 0.0f) / bufferSize;
    }

    // Berechne die Standardabweichung der Werte im Puffer
    float calculateStdDev(float mean) {
        float sumSqDiff = 0.0;
        for (float value : buffer) {
            sumSqDiff += std::pow(value - mean, 2);
        }
        return std::sqrt(sumSqDiff / bufferSize);
    }

    // Füge einen neuen Wert hinzu und berechne den gefilterten Wert
    float processValue(float newValue) {
        // Wenn der Puffer noch nicht voll ist, fülle ihn mit dem neuen Wert
        buffer[index] = newValue;
        index = (index + 1) % bufferSize;

        // Prüfe, ob der Puffer vollständig gefüllt ist
        if (index == 0) {
            bufferFilled = true;
        }

        // Berechne den Mittelwert und die Standardabweichung, wenn der Puffer gefüllt ist
        float mean = 0.0;
        float stddev = 0.0;
        if (bufferFilled) {
            mean = calculateMean();
            stddev = calculateStdDev(mean);
            
            // Berechne den adaptiven Schwellenwert (z.B. 2x die Standardabweichung)
            adaptiveThreshold = 2.0 * stddev;  // Schwellenwert als 2x die Standardabweichung
        }

        // Prüfe, ob der neue Wert ein Ausreißer ist (abweichend vom Mittelwert)
        if (bufferFilled && std::abs(newValue - mean) > adaptiveThreshold) {
            // Wert ist ein Ausreißer, verwende stattdessen den Mittelwert
            newValue = mean;
        }

        // Rückgabe des gefilterten Werts
        return mean;
    }
};


 */