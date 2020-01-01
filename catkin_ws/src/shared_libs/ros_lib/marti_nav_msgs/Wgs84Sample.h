#ifndef _ROS_marti_nav_msgs_Wgs84Sample_h
#define _ROS_marti_nav_msgs_Wgs84Sample_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Point.h"

namespace marti_nav_msgs
{

  class Wgs84Sample : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef geometry_msgs::Point _odom_type;
      _odom_type odom;
      typedef geometry_msgs::Point _wgs84_type;
      _wgs84_type wgs84;
      double wgs84_covariance[9];

    Wgs84Sample():
      header(),
      odom(),
      wgs84(),
      wgs84_covariance()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->odom.serialize(outbuffer + offset);
      offset += this->wgs84.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 9; i++){
      union {
        double real;
        uint64_t base;
      } u_wgs84_covariancei;
      u_wgs84_covariancei.real = this->wgs84_covariance[i];
      *(outbuffer + offset + 0) = (u_wgs84_covariancei.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wgs84_covariancei.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wgs84_covariancei.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wgs84_covariancei.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_wgs84_covariancei.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_wgs84_covariancei.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_wgs84_covariancei.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_wgs84_covariancei.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->wgs84_covariance[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->odom.deserialize(inbuffer + offset);
      offset += this->wgs84.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 9; i++){
      union {
        double real;
        uint64_t base;
      } u_wgs84_covariancei;
      u_wgs84_covariancei.base = 0;
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_wgs84_covariancei.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->wgs84_covariance[i] = u_wgs84_covariancei.real;
      offset += sizeof(this->wgs84_covariance[i]);
      }
     return offset;
    }

    const char * getType(){ return "marti_nav_msgs/Wgs84Sample"; };
    const char * getMD5(){ return "5241cb1ac52fc7a58f710c9a9774badc"; };

  };

}
#endif
