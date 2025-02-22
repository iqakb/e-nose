#include <Arduino.h>
#include <math.h>
//Pins declaration
const int pinSensorMQ135 = A4;
const int pinSensorMQ136 = A8;
const int pinSensorMQ137 = A9;
const int pinSensorMQ138 = A10;
const int pinSensorMQ2 = A0;
const int pinSensorMQ3 = A1;
const int pinSensorMQ4 = A2;
const int pinSensorMQ5 = A3;
const int pinSensorMQ6 = A7;
const int pinSensorMQ8 = A6;
const int pinSensorMQ9 = A5;
const int Excellent = 53;

// Sensor data declaration
int dataSensorMQ135;
int dataSensorMQ136;
int dataSensorMQ137;
int dataSensorMQ138;
int dataSensorMQ2;
int dataSensorMQ3;
int dataSensorMQ4;
int dataSensorMQ5;
int dataSensorMQ6;
int dataSensorMQ8;
int dataSensorMQ9;


void setup() {
  pinMode(pinSensorMQ135, INPUT);
  pinMode(pinSensorMQ136, INPUT);
  pinMode(pinSensorMQ137, INPUT);
  pinMode(pinSensorMQ138, INPUT);
  pinMode(pinSensorMQ2, INPUT);
  pinMode(pinSensorMQ3, INPUT);
  pinMode(pinSensorMQ4, INPUT);
  pinMode(pinSensorMQ5, INPUT);
  pinMode(pinSensorMQ6, INPUT);
  pinMode(pinSensorMQ8, INPUT);
  pinMode(pinSensorMQ9, INPUT);

  Serial.begin(9600);

}

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// ReLU activation function
double relu(double x) {
    return x > 0 ? x : 0;
}

void scale(double input[11]){
  for (size_t i=0;i<11;i++){
    input[i] = 0.1*input[1];
  }
}


void normalize(double input[11], double minval, double maxval){
  for (size_t i=0;i<11;i++){
    input[i] = 2*(input[i]-minval)/(maxval-minval) - 1;
  }
}

// Forward pass through the network
double forward_pass(double input[11]) {
    scale(input);
    normalize(input,2.4,54.08);
    // Layer 1 weights and biases
    double weights_1[11][11] = {
        {-0.265287369, -1.27115512, 0.939582944, -0.890317857, 1.44694197, 1.93205559, -0.880651057, 1.23062992, 0.0159381647, -0.332673758, -0.477678835},
        {3.61963511, -0.345256478, -1.12425041, -1.16525984, -0.591509461, 1.23600745, 0.960352421, -0.202370986, -0.462805539, 1.0796982, 0.804597735},
        {-0.219792977, 0.9348225, -1.70175183, 1.42832208, -1.29374266, -0.438634038, 1.15971446, -1.32825196, 1.69425142, 1.20822775, 1.38440299},
        {-2.1437788, -0.911462307, 1.28679502, -1.43151629, 1.51552141, 1.10685575, -1.17188406, 1.48467219, -0.5928455, -0.900194883, -0.421331704},
        {0.727387965, -0.401304096, 0.102236055, -0.856765449, 0.345567316, 0.764183998, -0.166355282, 0.495008349, 0.723972082, 0.520481765, -0.386937588},
        {-2.02838039, -0.116983667, 0.57869041, -0.933242917, 0.959063888, 1.01086283, -0.868147433, 0.267919064, 0.186064318, -0.20820117, -0.450464338},
        {-2.1168499, 0.173070982, 2.34830689, 0.640412927, -0.000919945422, -0.85681802, -0.676225722, 0.148345381, -2.88748431, -3.42122507, -1.48922253},
        {-0.811706245, 0.20678477, -0.61930424, 0.921005964, -0.150477424, -1.07057893, -0.258760929, -0.95120424, 0.749875486, 0.660366595, 0.239213094},
        {-0.653854609, 0.141817167, -0.78113997, 0.306187391, 0.724503577, -0.211510926, 0.522524834, 0.329084575, 2.02231383, 0.29743737, 0.734690249},
        {2.58440852, -0.837232411, 1.38464308, -1.49417686, -0.674210966, 0.175172225, 0.394064009, 0.454961181, -0.388638884, -2.26264668, -0.457418501},
        {-1.37466455, -0.284676611, 0.213381141, -0.874179244, 0.0542750657, 0.391391635, -0.115906477, 0.776998639, -0.0752557889, 0.245070755, 0.0401430279}
    };
    double biases_1[11] = {-2.930769, -0.22820468, 1.7020587, -0.05690896, 1.540833, 0.65120393, -0.42264527, 0.9353919, -0.8927208, -1.7560862, -0.58123857};

    // Layer 2 weights and biases
    double weights_2[11] ={2.8612835, 0.77727985, -1.4344898, 1.4233959, -1.3196362, -0.7573252,1.5106426,-1.0634665, 0.9804284, 1.4795765, 0.8365746 };
    double biases_2 = 0.39169696;

    // Forward pass through the first layer (sigmoid activation)
    double layer1_output[11];
    for (int i = 0; i < 11; i++) {
        layer1_output[i] = 0;
        printf("pass: %d\n",i);
        for (int j = 0; j < 11; j++) {
            printf("%f*%f=%f\n ",input[j], weights_1[j][i],input[j] * weights_1[j][i]);
            layer1_output[i] += input[j] * weights_1[j][i];
        }
        layer1_output[i] = sigmoid(layer1_output[i]+biases_1[i]);
    }

    // Forward pass through the second layer (ReLU activation)
    double output = 0;
    for (int i = 0; i < 11; i++) {
        output += layer1_output[i] * weights_2[i];
    }
    output = relu(output+biases_2);

    return output;
}


void loop() {
  dataSensorMQ135 = analogRead(pinSensorMQ135);
  dataSensorMQ136 = analogRead(pinSensorMQ136);
  dataSensorMQ137 = analogRead(pinSensorMQ137);
  dataSensorMQ138 = analogRead(pinSensorMQ138);
  dataSensorMQ2 = analogRead(pinSensorMQ2);
  dataSensorMQ3 = analogRead(pinSensorMQ3);
  dataSensorMQ4 = analogRead(pinSensorMQ4);
  dataSensorMQ5 = analogRead(pinSensorMQ5);
  dataSensorMQ6 = analogRead(pinSensorMQ6);
  dataSensorMQ8 = analogRead(pinSensorMQ8);
  dataSensorMQ9 = analogRead(pinSensorMQ9);

  Serial.print("MQ135: ");
  Serial.println(dataSensorMQ135);
  Serial.print("MQ136: ");
  Serial.println(dataSensorMQ136);
  Serial.print("MQ137: ");
  Serial.println(dataSensorMQ137);
  Serial.print("MQ138: ");
  Serial.println(dataSensorMQ138);
  Serial.print("MQ2: ");
  Serial.println(dataSensorMQ2);
  Serial.print("MQ3: ");
  Serial.println(dataSensorMQ3);
  Serial.print("MQ4: ");
  Serial.println(dataSensorMQ4);
  Serial.print("MQ5: ");
  Serial.println(dataSensorMQ5);
  Serial.print("MQ6: ");
  Serial.println(dataSensorMQ6);
  Serial.print("MQ8: ");
  Serial.println(dataSensorMQ8);
  Serial.print("MQ9: ");
  Serial.println(dataSensorMQ9);
  Serial.println();

  double input[11]={dataSensorMQ135, dataSensorMQ136 ,dataSensorMQ137 ,dataSensorMQ138     , dataSensorMQ2   , dataSensorMQ3, dataSensorMQ4, dataSensorMQ5,dataSensorMQ6, dataSensorMQ8, dataSensorMQ9};

  double output = forward_pass(input);
  Serial.print("TVC: ");
  Serial.println(output);

  delay(1000);
}
