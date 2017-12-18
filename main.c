#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
#include "test_vectors.h"
#include "message.h"
#include "frame.h"

#define FRAME_MARKER_START 0xF1
#define FRAME_MARKER_END 0xF2
#define FRAME_MARKER_ESCAPE 0xF3

#define FRAME_MAX_LENGTH 131070

bool command_received = false;
uint8_t uart_rcv_buffer[50];
int uart_rcv_buffer_lenght;
/* Temperary receiving buffer for incomming serial data*/
uint8_t rx_data[2];
/* Number of bytes received on serial */
int rx_indx = 0;
uint8_t rx_last[2] = {0x00, 0x00};
/* Final length of serial received data */
int message_len = 0;
/* Receiving buffer for incomming serial data */
volatile uint8_t rx_buffer[100];
/* True when escape received before character that should be escaped */
bool is_frame_marker_escape_received = false;

int x_test_vectors[13] = {-5585, -5504, -5623, -5647, -5671, -5514, -5383, -5657, -5706, -5400, -5455, -5704, -5574};
int y_test_vectors[13] = { 2485,  2460,  2405,  2445,  2325,  2515,  2410,  2440,  2415,  2445,  2470,  2250,  2455};

bool total_pass = true;
unsigned int receive_fail_counter = 0;
unsigned int length_fail_counter  = 0;
unsigned int data_fail_counter    = 0;

/* message generator variables */
/* Sending message */
message_t msg_send;
tlv_motor_position_t position_test;

int main(){
    printf("Hello world!\n");

    printf("*********************");
    printf("START TEST");
    printf("*********************");
    printf("\n");
    printf("\n");

    for(int i = -6000; i < 5000; i++){
    for(int j = 0; j < 5000; j++)
    {
    //for(int i = 0; i < 13; i++){
      printf("Test vector:");
      printf("\n");

      position_test.x = i;
      position_test.y = j;
      //position_test.x = x_test_vectors[i];
      //position_test.y = y_test_vectors[i];
      position_test.z = 0;

      message_init(&msg_send);
      message_tlv_add_command(&msg_send, COMMAND_MOVE_MOTOR);
      message_tlv_add_motor_position(&msg_send, &position_test);
      message_tlv_add_checksum(&msg_send);
      message_print(&msg_send);

      static uint8_t buffer[FRAME_MAX_LENGTH];
      ssize_t size = frame_message(buffer, sizeof(buffer), &msg_send);
      if (size < 0) {
        return -1;
      }
      printf("\n");
      print_vector(&buffer, (int)size, 1);

      /* Run the receive test function*/
      test_function(&buffer, (int)size);

      message_free(&msg_send);

      printf("Receive fail counter = %d \n", receive_fail_counter);
      printf("Length fail counter = %d \n", length_fail_counter);
      printf("Data fail counter = %d \n", data_fail_counter);

    }
    }

    if (total_pass != true)
    {
        printf("Al least one fail occured during testing!");
    }

    printf("Total Receive fail counter = %d \n", receive_fail_counter);
    printf("Total Length fail counter = %d \n", length_fail_counter);
    printf("Total Data fail counter = %d \n", data_fail_counter);

    return 0;
}


/**
   Test function
*/
void test_function(uint8_t *vector, int vector_size){


  /* Run test function for each vector */
  /*********************DUT**********************/
  receive_bytes(vector, vector_size);
  /**********************************************/
  printf("-------------------------------------");
  printf("\n");
  /* Check if command was received */
  if(command_received == true){
    printf("RECEIVE:");
    printf("\t\t");
    printf("PASS");
  }else{
    total_pass = false;
    receive_fail_counter++;
    printf("RECEIVE:");
    printf("\t\t");
    printf("FAIL");
  }
  printf("\n");

  /* Check the length size returned */
  if(message_len == vector_size){
    printf("LENGTH: ");
    printf("\t\t");
    printf("PASS");
  }else{
    total_pass = false;
    length_fail_counter++;
    printf("LENGTH: ");
    printf("\t\t");
    printf("FAIL  %d", message_len);
  }
  printf("\n");
  /* Check if the message is received correctly */
  printf("DATA:   ");
  printf("\t\t");
  bool data_ok = true;
  for(int i = 0; i < vector_size; i++){
    if(vector[i] != rx_buffer[i]){
      bool data_ok = false;
    }
  }
  if(data_ok == false){
    total_pass = false;
    data_fail_counter++;
    printf("FAIL");
    printf("\n");
    printf("received vector: ");
    printf("\n");
    print_vector(&rx_buffer, vector_size, 1);
  }else{
    printf("PASS");
  }
  printf("\n");

  /* reset values */
  data_ok = true;
  command_received = false;
/* Return report */
}

/**
    Print vector
*/
void print_vector(uint8_t *vector, int vector_size, int select){
  if(select == 0){
    for(int i = 1; i <= vector_size; i++){
      printf("%02X ", vector[i]);
    }
    printf("\n");
  }else if(select == 1){
    for(int i = 0; i < vector_size; i++){
    printf("%02X ", vector[i]);
    }
    printf("\n");
  }
}

/**
   Handles receivign bytes from UART.
*/
void receive_bytes(uint8_t *vector,int vector_size)
{
  //debugSerial.println("receiving bytes");
  for(int y = 0; y < vector_size; y++){//while (Serial.available()) {
    rx_data[0] = vector[y];
        /* Clear Rx_Buffer before receiving new data */
        //rx_indx not initialized at the beginning - it should be initialized to 0

        /* Start byte received */
        if (rx_data[0] == FRAME_MARKER_START)
        {
            /* Start byte received in the frame */
            //if ((rx_last[0] == FRAME_MARKER_ESCAPE) && (rx_buffer[0] == FRAME_MARKER_START))
            if ((is_frame_marker_escape_received == true) && (rx_buffer[0] == FRAME_MARKER_START))
            {
                //F1........F3F1
                rx_buffer[rx_indx++] = rx_data[0];
            }
            /* Real start byte received */
            else
            {
                //F1........xxF1
                //xx........F3F1
                //xx........xxF1
                rx_indx = 0;
                // Clear buffer since start of the new frame is detected
                for (int i = 0; i < 100; i++)
                {
                    rx_buffer[i] = 0;
                }
                // Store start marker to buffer
                rx_buffer[rx_indx++] = rx_data[0];
            }

            //is_frame_marker_escape_received = false;
            /**
             * Check other invalid situation
             */
        }
        /* End byte received */
        else if (rx_data[0] == FRAME_MARKER_END)
        {
            /* End byte received in the frame */
            if ((is_frame_marker_escape_received == true) && (rx_buffer[0] == FRAME_MARKER_START))
            {
                //F1........F3F2
                rx_buffer[rx_indx++] = rx_data[0];
            }
            /* Real end byte received */
            // End cannot be received if valid frame is not started
            else if (rx_buffer[0] == FRAME_MARKER_START)
            {
                //F1........xxF2
                rx_buffer[rx_indx++] = rx_data[0];
                message_len = rx_indx;
                rx_indx = 0;
                rx_last[0] = 0;
                rx_last[1] = 0;
                /* Transfer complete, data is ready to read */
                command_received = true;
                /* Disable USART1 interrupt */
            }
            else
            {
                //xx........xxF2
                //xx........F3F2
                //We do not care about this?
            }
            //is_frame_marker_escape_received = false;
        }
        else
        {
            // if FRAME is valid
            if (rx_buffer[0] == FRAME_MARKER_START)
            {
                //put received character in the buffer
                rx_buffer[rx_indx++] = rx_data[0];
            }
        }

        //Handle ESC frame check
        if ((rx_data[0] == FRAME_MARKER_ESCAPE) && (is_frame_marker_escape_received == false))
        {
            is_frame_marker_escape_received = true;
        }
        else
        {
            is_frame_marker_escape_received = false;
        }
  }
}
