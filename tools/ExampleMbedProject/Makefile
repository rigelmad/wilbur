# Copyright 2017 Adam Green (http://mbed.org/users/AdamGreen/)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# NOTE: This requires you have `ROVER_WS` exported as the path to `rover_2020/`

PROJECT         := ExampleMbedProject
DEVICES         := NUCLEO_F410RB
GCC4MBED_DIR    := ${ROVER_WS}/tools/gcc4mbed/
SHARED_LIBS_DIR	:= ${ROVER_WS}/catkin_ws/src/shared_libs
NO_FLOAT_SCANF  := 1
NO_FLOAT_PRINTF := 1

# Include ROS libraries if necessary, or anything else from shared libs
USER_LIBS       := $(SHARED_LIBS_DIR)/ros_lib \
										./libs/ExampleLib # Include local libraries

# Include the master gcc4mbed makefile
include $(GCC4MBED_DIR)/build/gcc4mbed.mk
