# Copyright (C) 2024 Gary Kim <gary@garykim.dev>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.20)

include(arch/rp2040/pico-sdk/pico_sdk_init.cmake)
project(gkrtos)

set(CMAKE_C_STANDARD 17)

pico_sdk_init()

set(INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/arch
        ${CMAKE_SOURCE_DIR}/src
)

set(SRC_FILES
        ${CMAKE_SOURCE_DIR}/src/gkrtos/asm.S
        ${CMAKE_SOURCE_DIR}/src/gkrtos/concurrency/spinlock.c
        ${CMAKE_SOURCE_DIR}/src/gkrtos/gkrtos.c
        ${CMAKE_SOURCE_DIR}/src/gkrtos/hardware/rp2040.c
        ${CMAKE_SOURCE_DIR}/src/gkrtos/interrupts/svcall.c
        ${CMAKE_SOURCE_DIR}/src/gkrtos/interrupts/systick.c
        ${CMAKE_SOURCE_DIR}/src/gkrtos/structures/list.c
        ${CMAKE_SOURCE_DIR}/src/gkrtos/tasking/tasking.c
)

add_executable(gkrtos
        ${SRC_FILES}
)
target_include_directories(gkrtos PUBLIC ${INCLUDE_DIRS})
target_link_libraries(gkrtos pico_stdlib hardware_exception)

pico_add_extra_outputs(gkrtos)

