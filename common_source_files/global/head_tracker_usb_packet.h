/*
 * head_tracker_usb_packet.h
 *
 *  Created on: Nov 1, 2020
 *      Author: Fongberg
 */

#ifndef HEAD_TRACKER_USB_PACKET_H_
#define HEAD_TRACKER_USB_PACKET_H_

typedef struct _USB_TrackerPacket_t {
  uint16_t x;
  uint16_t y;
  uint16_t z;
  uint16_t pitch; //rx
  uint16_t roll; //ry
  uint16_t yaw; //rz
} USB_TrackerPacket_t;

#endif /* HEAD_TRACKER_USB_PACKET_H_ */
