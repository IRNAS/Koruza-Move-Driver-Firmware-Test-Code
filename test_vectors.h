#ifndef TEST_VECTORS_H_INCLUDED
#define TEST_VECTORS_H_INCLUDED

uint8_t test_vector_get_status[4][30] = {
  /*get_status*/
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2}
};

uint8_t test_vector_homing[4][30] = {
  /*get_status*/
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x06, 0x03, 0x00, 0x04, 0x3B, 0x61, 0x4A, 0xB8, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2}
};

uint8_t test_vector_move_motors_1[4][30] = {
  /*get_status*/
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2},
  { 13, 0xF1, 0x01, 0x00, 0x01, 0x01, 0x03, 0x00, 0x04, 0xA5, 0x05, 0xDF, 0x1B, 0xF2}
};
int test_vector_size[] = {

};
#endif // TEST_VECTORS_H_INCLUDED