#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/robot.h>
#include <stdio.h>
#include <webots/device.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/nodes.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <math.h>

#define TIME_STEP 64
#define QtddSensoresProx 8
#define QtddLeds 10
#define MAX_SPEED 5

int main(int argc, char **argv) {

  int i=0;
  char texto[256];
  double LeituraSensorProx[QtddSensoresProx];
  double AceleradorDireito=1.0, AceleradorEsquerdo=1.0;
  double ps_threshold = 100.0;
  double pushing_distance_threshold = 0.01; // A distância que a caixa deve ser movida para acender os LEDs
  double initial_robot_position[3];
  double initial_box_position[3];
  bool is_pushing_box = false;

  for(i=0;i<257;i++) texto[i]='0';

  wb_robot_init();

  WbDeviceTag MotorEsquerdo, MotorDireito;
  WbNodeRef robot_node = wb_supervisor_node_get_from_def("EPUCK");
  WbNodeRef box_node = wb_supervisor_node_get_from_def("BOX");

  MotorEsquerdo = wb_robot_get_device("left wheel motor");
  MotorDireito = wb_robot_get_device("right wheel motor");

  wb_motor_set_position(MotorEsquerdo, INFINITY);
  wb_motor_set_position(MotorDireito, INFINITY);
  wb_motor_set_velocity(MotorEsquerdo,0);
  wb_motor_set_velocity(MotorDireito,0);

  WbDeviceTag SensorProx[QtddSensoresProx];

  for(i=0; i<QtddSensoresProx; i++) {
    char sensor_name[4];
    sprintf(sensor_name, "ps%d", i);
    SensorProx[i] = wb_robot_get_device(sensor_name);
    wb_distance_sensor_enable(SensorProx[i],TIME_STEP);
  }

  WbDeviceTag Leds[QtddLeds];
  Leds[0] = wb_robot_get_device("led0");
  Leds[1] = wb_robot_get_device("led1");
  Leds[2] = wb_robot_get_device("led2");
  Leds[3] = wb_robot_get_device("led3");
  Leds[4] = wb_robot_get_device("led4");
  Leds[5] = wb_robot_get_device("led5");
  Leds[6] = wb_robot_get_device("led6");
  Leds[7] = wb_robot_get_device("led7");
  Leds[8] = wb_robot_get_device("led8");
  Leds[9] = wb_robot_get_device("led9");
  wb_led_set(Leds[0],-1);
  wb_led_set(Leds[1],-1);
  wb_led_set(Leds[2],-1);
  wb_led_set(Leds[3],-1);
  wb_led_set(Leds[4],-1);
  wb_led_set(Leds[5],-1);
  wb_led_set(Leds[6],-1);
  wb_led_set(Leds[7],-1);
  wb_led_set(Leds[8],-1);
  wb_led_set(Leds[9],-1);

  // Obtenha a posição inicial do robô e da caixa
  const double *position = wb_supervisor_node_get_position(robot_node);
  memcpy(initial_robot_position, position, sizeof(initial_robot_position));
  position = wb_supervisor_node_get_position(box_node);
  memcpy(initial_box_position, position, sizeof(initial_box_position));

  while (wb_robot_step(TIME_STEP) != -1) {
    for(i=0;i<256;i++) texto[i]=0;

    for(i=0;i<QtddSensoresProx;i++){
       LeituraSensorProx[i]= wb_distance_sensor_get_value(SensorProx[i])-60;
       sprintf(texto,"%s|%d: %5.2f  ",texto,i,LeituraSensorProx[i]);
    }

    printf("%s\n",texto);
    wb_led_set(Leds[0], wb_led_get(Leds[0])*-1);

    // Vá para frente por padrão
    AceleradorEsquerdo = AceleradorDireito = 1.0;
    
    // Se detectar um obstáculo à frente, vire à esquerda ou à direita
    if (LeituraSensorProx[7] > ps_threshold || LeituraSensorProx[0] > ps_threshold) {
        if (LeituraSensorProx[7] > LeituraSensorProx[0]) {
            AceleradorEsquerdo = 1.0; // Vire à direita
            AceleradorDireito = -1.0;
        } else {
            AceleradorEsquerdo = -1.0; // Vire à esquerda
            AceleradorDireito = 1.0;
        }
    }
    
    // Se detectar um obstáculo à direita, vire à esquerda
    if (LeituraSensorProx[2] > ps_threshold) {
        AceleradorEsquerdo = -1.0;
        AceleradorDireito = 1.0;
    }
    
    // Se detectar um obstáculo à esquerda, vire à direita
    if (LeituraSensorProx[5] > ps_threshold) {
        AceleradorEsquerdo = 1.0;
        AceleradorDireito = -1.0;
    }
    
    // Se detectar um obstáculo atrás, continue indo para frente
    if (LeituraSensorProx[4] > ps_threshold || LeituraSensorProx[3] > ps_threshold) {
        AceleradorEsquerdo = AceleradorDireito = 1.0;
    }
    
    // Obtenha a posição atual do robô e da caixa
    position = wb_supervisor_node_get_position(robot_node);
    double current_robot_position[3];
    memcpy(current_robot_position, position, sizeof(current_robot_position));
    position = wb_supervisor_node_get_position(box_node);
    double current_box_position[3];
    memcpy(current_box_position, position, sizeof(current_box_position));
    
    // Verifique se o robô está empurrando a caixa
    if (!is_pushing_box && (fabs(current_robot_position[0] - current_box_position[0]) < 0.1 || fabs(current_robot_position[2] - current_box_position[2]) < 0.1)) {
        is_pushing_box = true;
        // Atualize a posição inicial do robô e da caixa
        memcpy(initial_robot_position, current_robot_position, sizeof(initial_robot_position));
        memcpy(initial_box_position, current_box_position, sizeof(initial_box_position));
    }
    
    // Se o robô estiver empurrando a caixa e a caixa tiver sido movida uma certa distância, acenda os LEDs e pare o robô
    if (is_pushing_box && fabs(current_box_position[0] - initial_box_position[0]) > pushing_distance_threshold || fabs(current_box_position[2] - initial_box_position[2]) > pushing_distance_threshold) {
        wb_led_set(Leds[0], 1);
        wb_led_set(Leds[1], 1);
        wb_led_set(Leds[2], 1);
        wb_led_set(Leds[3], 1);
        wb_led_set(Leds[4], 1);
        wb_led_set(Leds[5], 1);
        wb_led_set(Leds[6], 1);
        wb_led_set(Leds[7], 1);
        wb_led_set(Leds[8], 1);
        wb_led_set(Leds[9], 1);
        AceleradorEsquerdo = AceleradorDireito = 0.0;
    }
    
    wb_motor_set_velocity(MotorEsquerdo, MAX_SPEED * AceleradorEsquerdo);
    wb_motor_set_velocity(MotorDireito, MAX_SPEED * AceleradorDireito);
    }

  wb_robot_cleanup();

  return 0;
}
