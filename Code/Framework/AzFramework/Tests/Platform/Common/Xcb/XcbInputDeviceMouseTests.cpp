/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <xcb/xcb.h>

#include <AzFramework/Input/Buses/Requests/InputSystemCursorRequestBus.h>
#include <AzFramework/Input/Devices/Mouse/InputDeviceMouse.h>

#include "XcbBaseTestFixture.h"
#include "XcbTestApplication.h"
#include "Matchers.h"
#include "Actions.h"

namespace AzFramework
{
    // Sets up default behavior for mock keyboard responses to xcb methods
    class XcbInputDeviceMouseTests
        : public XcbBaseTestFixture
    {
    public:
        void SetUp() override
        {
            using testing::Return;
            using testing::_;

            XcbBaseTestFixture::SetUp();

            ON_CALL(m_interface, xcb_get_setup(&m_connection))
                .WillByDefault(Return(&s_xcbSetup));
            ON_CALL(m_interface, xcb_setup_roots_iterator(&s_xcbSetup))
                .WillByDefault(Return(xcb_screen_iterator_t{&s_xcbScreen}));

            ON_CALL(m_interface, xcb_get_extension_data(&m_connection, &xcb_xfixes_id))
                .WillByDefault(Return(&s_xfixesExtensionReply));
            ON_CALL(m_interface, xcb_xfixes_query_version_reply(&m_connection, _, _))
                .WillByDefault(ReturnMalloc<xcb_xfixes_query_version_reply_t>(
                    /*response_type=*/(uint8_t)XCB_XFIXES_QUERY_VERSION,
                    /*pad0=*/(uint8_t)0,
                    /*sequence=*/(uint16_t)1,
                    /*length=*/0u,
                    /*major_version=*/5u,
                    /*minor_version=*/0u
                ));

            ON_CALL(m_interface, xcb_get_extension_data(&m_connection, &xcb_input_id))
                .WillByDefault(Return(&s_xfixesExtensionReply));
            ON_CALL(m_interface, xcb_input_xi_query_version_reply(&m_connection, _, _))
                .WillByDefault(ReturnMalloc<xcb_input_xi_query_version_reply_t>(
                    /*response_type=*/(uint8_t)XCB_INPUT_XI_QUERY_VERSION,
                    /*pad0=*/(uint8_t)0,
                    /*sequence=*/(uint16_t)1,
                    /*length=*/0u,
                    /*major_version=*/(uint16_t)2,
                    /*minor_version=*/(uint16_t)2
                ));
        }

        void PumpApplication()
        {
            m_application.PumpSystemEventLoopUntilEmpty();
            m_application.TickSystem();
            m_application.Tick();
        }

    protected:
        static constexpr inline uint8_t s_xinputMajorOpcode = 131;
        static constexpr inline xcb_window_t s_rootWindow = 1;
        static constexpr inline xcb_input_device_id_t s_virtualCorePointerId = 2;
        static constexpr inline xcb_input_device_id_t s_physicalPointerDeviceId = 3;
        static constexpr inline uint16_t s_screenWidthInPixels = 3840;
        static constexpr inline uint16_t s_screenHeightInPixels = 2160;
        static constexpr inline xcb_setup_t s_xcbSetup{
            /*.status=*/1,
            /*.pad0=*/0,
            /*.protocol_major_version=*/11,
            /*.protocol_minor_version=*/0,
        };
        static inline xcb_screen_t s_xcbScreen{
            /*.root=*/s_rootWindow,
            /*.default_colormap=*/32,
            /*.white_pixel=*/16777215,
            /*.black_pixel=*/0,
            /*.current_input_masks=*/0,
            /*.width_in_pixels=*/s_screenWidthInPixels,
            /*.height_in_pixels=*/s_screenHeightInPixels,
            /*.width_in_millimeters=*/602,
            /*.height_in_millimeters=*/341,
        };
        static constexpr inline xcb_query_extension_reply_t s_xfixesExtensionReply{
            /*.response_type=*/XCB_QUERY_EXTENSION,
            /*.pad0=*/0,
            /*.sequence=*/1,
            /*.length=*/0,
            /*.present=*/1,
        };
        static constexpr inline xcb_query_extension_reply_t s_xinputExtensionReply{
            /*.response_type=*/XCB_QUERY_EXTENSION,
            /*.pad0=*/0,
            /*.sequence=*/1,
            /*.length=*/0,
            /*.present=*/1,
            /*.major_opcode=*/s_xinputMajorOpcode,
        };
        XcbTestApplication m_application{
            /*enabledGamepadsCount=*/0,
            /*keyboardEnabled=*/false,
            /*motionEnabled=*/false,
            /*mouseEnabled=*/true,
            /*touchEnabled=*/false,
            /*virtualKeyboardEnabled=*/false
        };
    };

    struct MouseButtonTestData
    {
        xcb_button_index_t m_button;
    };

    class XcbInputDeviceMouseButtonTests
        : public XcbInputDeviceMouseTests
        , public testing::WithParamInterface<MouseButtonTestData>
    {
    public:
        static InputChannelId GetInputChannelIdForButton(const xcb_button_index_t button)
        {
            switch (button)
            {
                case XCB_BUTTON_INDEX_1:
                    return InputDeviceMouse::Button::Left;
                case XCB_BUTTON_INDEX_2:
                    return InputDeviceMouse::Button::Right;
                case XCB_BUTTON_INDEX_3:
                    return InputDeviceMouse::Button::Middle;
            }
            return InputChannelId{};
        }

        AZStd::array<InputChannelId, 4> GetIdleChannelIdsForButton(const xcb_button_index_t button)
        {
            switch (button)
            {
            case XCB_BUTTON_INDEX_1:
                return { InputDeviceMouse::Button::Right, InputDeviceMouse::Button::Middle, InputDeviceMouse::Button::Other1, InputDeviceMouse::Button::Other2 };
            case XCB_BUTTON_INDEX_2:
                return { InputDeviceMouse::Button::Left, InputDeviceMouse::Button::Middle, InputDeviceMouse::Button::Other1, InputDeviceMouse::Button::Other2 };
            case XCB_BUTTON_INDEX_3:
                return { InputDeviceMouse::Button::Left, InputDeviceMouse::Button::Right, InputDeviceMouse::Button::Other1, InputDeviceMouse::Button::Other2 };
            case XCB_BUTTON_INDEX_4:
                return { InputDeviceMouse::Button::Left, InputDeviceMouse::Button::Right, InputDeviceMouse::Button::Middle, InputDeviceMouse::Button::Other2 };
            case XCB_BUTTON_INDEX_5:
                return { InputDeviceMouse::Button::Left, InputDeviceMouse::Button::Right, InputDeviceMouse::Button::Middle, InputDeviceMouse::Button::Other1 };
            }
            return AZStd::array<InputChannelId, 4>();
        }
    };

    TEST_P(XcbInputDeviceMouseButtonTests, ButtonInputChannelsUpdateStateFromXcbEvents)
    {
        using testing::Each;
        using testing::Eq;
        using testing::NotNull;
        using testing::Property;
        using testing::Return;

        // Set the expectations for the events that will be generated
        // nullptr entries represent when the event queue is empty, and will cause
        // PumpSystemEventLoopUntilEmpty to return
        //
        // Event pointers are freed by the calling code, so these actions
        // malloc new copies
        //
        // The xcb mouse does not react to the `XCB_BUTTON_PRESS` /
        // `XCB_BUTTON_RELEASE` events, but it will still receive those events
        // from the X server.
        EXPECT_CALL(m_interface, xcb_poll_for_event(&m_connection))
            .WillOnce(ReturnMalloc<xcb_generic_event_t>(MakeEvent(xcb_input_raw_button_press_event_t{
                /*response_type=*/XCB_GE_GENERIC,
                /*extension=*/s_xinputMajorOpcode,
                /*sequence=*/4,
                /*length=*/2,
                /*event_type=*/XCB_INPUT_RAW_BUTTON_PRESS,
                /*deviceid=*/s_virtualCorePointerId,
                /*time=*/3984920,
                /*detail=*/GetParam().m_button,
                /*sourceid=*/s_physicalPointerDeviceId,
                /*valuators_len=*/2,
                /*flags=*/0,
                /*pad0[4]=*/{},
                /*full_sequence=*/4
            })))
            .WillOnce(Return(nullptr))
            .WillOnce(ReturnMalloc<xcb_generic_event_t>(MakeEvent(xcb_button_press_event_t{
                /*response_type=*/XCB_BUTTON_PRESS,
                /*detail=*/static_cast<xcb_button_t>(GetParam().m_button),
                /*sequence=*/4,
                /*time=*/3984920,
                /*root=*/s_rootWindow,
                /*event=*/119537664,
                /*child=*/0,
                /*root_x=*/55,
                /*root_y=*/1099,
                /*event_x=*/55,
                /*event_y=*/55,
                /*state=*/0,
                /*same_screen=*/1
            })))
            .WillOnce(Return(nullptr))
            .WillOnce(ReturnMalloc<xcb_generic_event_t>(MakeEvent(xcb_input_raw_button_release_event_t{
                /*response_type=*/XCB_GE_GENERIC,
                /*extension=*/s_xinputMajorOpcode,
                /*sequence=*/4,
                /*length=*/2,
                /*event_type=*/XCB_INPUT_RAW_BUTTON_RELEASE,
                /*deviceid=*/s_virtualCorePointerId,
                /*time=*/3984964,
                /*detail=*/GetParam().m_button,
                /*sourceid=*/s_physicalPointerDeviceId,
                /*valuators_len=*/2,
                /*flags=*/0,
                /*pad0[4]=*/{},
                /*full_sequence=*/4
            })))
            .WillOnce(Return(nullptr))
            .WillOnce(ReturnMalloc<xcb_generic_event_t>(MakeEvent(xcb_button_release_event_t{
                /*response_type=*/XCB_BUTTON_RELEASE,
                /*detail=*/static_cast<xcb_button_t>(GetParam().m_button),
                /*sequence=*/4,
                /*time=*/3984964,
                /*root=*/s_rootWindow,
                /*event=*/119537664,
                /*child=*/0,
                /*root_x=*/55,
                /*root_y=*/1099,
                /*event_x=*/55,
                /*event_y=*/55,
                /*state=*/XCB_KEY_BUT_MASK_BUTTON_1,
                /*same_screen=*/1
            })))
            .WillOnce(Return(nullptr))
            ;

        m_application.Start();
        InputSystemCursorRequestBus::Event(
            InputDeviceMouse::Id,
            &InputSystemCursorRequests::SetSystemCursorState,
            SystemCursorState::ConstrainedAndHidden);

        const InputChannel* activeButtonChannel = InputChannelRequests::FindInputChannel(GetInputChannelIdForButton(GetParam().m_button));
        const auto inactiveButtonChannels = [this]()
        {
            const auto inactiveButtonChannelIds = GetIdleChannelIdsForButton(GetParam().m_button);
            AZStd::array<const InputChannel*, 4> channels{};
            AZStd::transform(begin(inactiveButtonChannelIds), end(inactiveButtonChannelIds), begin(channels), [](const InputChannelId& id)
            {
                return InputChannelRequests::FindInputChannel(id);
            });
            return channels;
        }();

        ASSERT_TRUE(activeButtonChannel);
        ASSERT_THAT(inactiveButtonChannels, Each(NotNull()));

        EXPECT_THAT(activeButtonChannel->GetState(), Eq(InputChannel::State::Idle));
        EXPECT_THAT(inactiveButtonChannels, Each(Property(&InputChannel::GetState, Eq(InputChannel::State::Idle))));

        PumpApplication();

        EXPECT_THAT(activeButtonChannel->GetState(), Eq(InputChannel::State::Began));
        EXPECT_THAT(inactiveButtonChannels, Each(Property(&InputChannel::GetState, Eq(InputChannel::State::Idle))));

        PumpApplication();

        EXPECT_THAT(activeButtonChannel->GetState(), Eq(InputChannel::State::Updated));
        EXPECT_THAT(inactiveButtonChannels, Each(Property(&InputChannel::GetState, Eq(InputChannel::State::Idle))));

        PumpApplication();

        EXPECT_THAT(activeButtonChannel->GetState(), Eq(InputChannel::State::Ended));
        EXPECT_THAT(inactiveButtonChannels, Each(Property(&InputChannel::GetState, Eq(InputChannel::State::Idle))));

        PumpApplication();

        EXPECT_THAT(activeButtonChannel->GetState(), Eq(InputChannel::State::Idle));
        EXPECT_THAT(inactiveButtonChannels, Each(Property(&InputChannel::GetState, Eq(InputChannel::State::Idle))));
    }

    INSTANTIATE_TEST_CASE_P(
        AllButtons,
        XcbInputDeviceMouseButtonTests,
        testing::Values(
            MouseButtonTestData{ XCB_BUTTON_INDEX_1 },
            MouseButtonTestData{ XCB_BUTTON_INDEX_2 },
            MouseButtonTestData{ XCB_BUTTON_INDEX_3 }
            // XCB_BUTTON_INDEX_4 and XCB_BUTTON_INDEX_5 map to positive and
            // negative scroll wheel events, which are handled as motion events
        )
    );

    TEST_F(XcbInputDeviceMouseTests, MovementInputChannelsUpdateStateFromXcbEvents)
    {
        using testing::Each;
        using testing::Eq;
        using testing::FloatEq;
        using testing::NotNull;
        using testing::Property;
        using testing::Return;

        // Set the expectations for the events that will be generated
        // nullptr entries represent when the event queue is empty, and will cause
        // PumpSystemEventLoopUntilEmpty to return
        //
        // Event pointers are freed by the calling code, so these actions
        // malloc new copies
        //
        // The xcb mouse does not react to the `XCB_MOTION_NOTIFY` event, but
        // it will still receive it from the X server.
        EXPECT_CALL(m_interface, xcb_poll_for_event(&m_connection))
            .WillOnce(ReturnMalloc<xcb_generic_event_t>(MakeEvent(xcb_input_raw_motion_event_t{
                /*response_type=*/XCB_GE_GENERIC,
                /*extension=*/s_xinputMajorOpcode,
                /*sequence=*/5,
                /*length=*/10,
                /*event_type=*/XCB_INPUT_RAW_MOTION,
                /*deviceid=*/s_virtualCorePointerId,
                /*time=*/0, // use the time value to identify each event
                /*detail=*/XCB_MOTION_NORMAL,
                /*sourceid=*/s_physicalPointerDeviceId,
                /*valuators_len=*/2, // number of axes that have values for this event
                /*flags=*/0,
                /*pad0[4]=*/{},
                /*full_sequence=*/5,
            })))
            .WillOnce(Return(nullptr))
            .WillOnce(ReturnMalloc<xcb_generic_event_t>(MakeEvent(xcb_motion_notify_event_t{
                /*response_type=*/XCB_MOTION_NOTIFY,
                /*detail=*/XCB_MOTION_NORMAL,
                /*sequence=*/5,
                /*time=*/1, // use the time value to identify each event
                /*root=*/s_rootWindow,
                /*event=*/127926272,
                /*child=*/0,
                /*root_x=*/95,
                /*root_y=*/1079,
                /*event_x=*/95,
                /*event_y=*/20,
                /*state=*/0,
                /*same_screen=*/1,
            })))
            .WillOnce(Return(nullptr))
            ;

        AZStd::array axisValues
        {
            xcb_input_fp3232_t{ /*.integral=*/ 1, /*.fraction=*/0 }, // x motion
            xcb_input_fp3232_t{ /*.integral=*/ 2, /*.fraction=*/0 }  // y motion
        };

        EXPECT_CALL(m_interface, xcb_input_raw_button_press_axisvalues_length(testing::Field(&xcb_input_raw_button_press_event_t::time, 0)))
            .WillRepeatedly(testing::Return(2)); // x and y axis
        EXPECT_CALL(m_interface, xcb_input_raw_button_press_axisvalues_raw(testing::Field(&xcb_input_raw_button_press_event_t::time, 0)))
            .WillRepeatedly(testing::Return(axisValues.data())); // x and y axis

        m_application.Start();
        InputSystemCursorRequestBus::Event(
            InputDeviceMouse::Id,
            &InputSystemCursorRequests::SetSystemCursorState,
            SystemCursorState::ConstrainedAndHidden);

        const InputChannel* xMotionChannel = InputChannelRequests::FindInputChannel(InputDeviceMouse::Movement::X);
        const InputChannel* yMotionChannel = InputChannelRequests::FindInputChannel(InputDeviceMouse::Movement::Y);
        ASSERT_TRUE(xMotionChannel);
        ASSERT_TRUE(yMotionChannel);

        EXPECT_THAT(xMotionChannel->GetState(), Eq(InputChannel::State::Idle));
        EXPECT_THAT(yMotionChannel->GetState(), Eq(InputChannel::State::Idle));
        EXPECT_THAT(xMotionChannel->GetValue(), FloatEq(0.0f));
        EXPECT_THAT(yMotionChannel->GetValue(), FloatEq(0.0f));

        PumpApplication();

        EXPECT_THAT(xMotionChannel->GetState(), Eq(InputChannel::State::Began));
        EXPECT_THAT(yMotionChannel->GetState(), Eq(InputChannel::State::Began));
        EXPECT_THAT(xMotionChannel->GetValue(), FloatEq(1.0f));
        EXPECT_THAT(yMotionChannel->GetValue(), FloatEq(2.0f));

        PumpApplication();

        EXPECT_THAT(xMotionChannel->GetState(), Eq(InputChannel::State::Ended));
        EXPECT_THAT(yMotionChannel->GetState(), Eq(InputChannel::State::Ended));
        EXPECT_THAT(xMotionChannel->GetValue(), FloatEq(0.0f));
        EXPECT_THAT(yMotionChannel->GetValue(), FloatEq(0.0f));
    }
} // namespace AzFramework
