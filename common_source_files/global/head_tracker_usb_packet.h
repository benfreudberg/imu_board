/*
 * head_tracker_usb_packet.h
 *
 *  Created on: Nov 1, 2020
 *      Author: Fongberg
 */

#ifndef HEAD_TRACKER_USB_PACKET_H_
#define HEAD_TRACKER_USB_PACKET_H_

typedef struct _USB_TrackerPacket_t {
  int16_t x;
  int16_t y;
  int16_t z;
  int16_t pitch; //rx
  int16_t roll; //ry
  int16_t yaw; //rz
} USB_TrackerPacket_t;

#endif /* HEAD_TRACKER_USB_PACKET_H_ */
