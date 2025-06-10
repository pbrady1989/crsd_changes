/* =========================================================================
 * This file is part of crsd-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2019, MDA Information Systems LLC
 *
 * crsd-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */
#include <crsd/Channel.h>

#include <memory>

#include <xml/lite/MinidomParser.h>
#include <six/Init.h>
#include <crsd/Enums.h>
#include <crsd/Types.h>

#include "TestCase.h"

TEST_CASE(EmptyChannel)
{
    crsd::Channel channel1, channel2;
    channel1.parameters.resize(3);
    channel2.parameters.resize(3);
    TEST_ASSERT_TRUE((channel1.parameters == channel2.parameters));
    TEST_ASSERT_TRUE((channel1 == channel2));
}

TEST_CASE(TestPolygonInvalid)
{
    crsd::Channel channel;
    channel.parameters.resize(1);
    channel.parameters[0].sarImage.reset(new crsd::ChannelSARImage());

    six::Vector2 vertex1, vertex2;
    vertex1[0] = 0;
    vertex1[1] = 0;
    vertex2[0] = 5;
    vertex2[1] = 5;

    channel.parameters[0].sarImage->imageArea.x1y1[0] = 0;
    channel.parameters[0].sarImage->imageArea.x1y1[1] = 0;
    channel.parameters[0].sarImage->imageArea.x2y2[0] = 10;
    channel.parameters[0].sarImage->imageArea.x2y2[1] = 10;
    channel.parameters[0].sarImage->imageArea.polygon.push_back(vertex1);
    channel.parameters[0].sarImage->imageArea.polygon.push_back(vertex2);

    TEST_ASSERT_FALSE(channel.parameters[0].sarImage->imageArea.polygon.size() >= 3);
}

TEST_MAIN(
        TEST_CHECK(EmptyChannel);
        TEST_CHECK(TestPolygonInvalid);
        )

