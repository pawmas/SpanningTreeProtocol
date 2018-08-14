// Tested project's headers
#include <stp/sm/bridge_detection.hpp>
// UT dependencies
#include "sut_machine.hpp"
#include <mock/bridge.hpp>
#include <mock/bridge_detection_sm.hpp>

// GTest headers
#include <gtest/gtest.h>

using namespace SpanningTree::BridgeDetection;

class BdmSutMachine : public SutMachine {
public:
    BdmSutMachine(sptr<Mock::Bridge> bridge, sptr<SpanningTree::Port> port)
        : SutMachine{ bridge, port }, _mockBridge{ bridge }, _port{ port } {}
    Mock::Bridge& BridgeInstance() const noexcept override {
        return *_mockBridge;
    }

private:
    sptr<Mock::Bridge> _mockBridge;
    sptr<SpanningTree::Port> _port;
};

class BridgeDetectionTest : public ::testing::Test {
protected:
    BridgeDetectionTest()
        : _sutMachine{ std::make_shared<Mock::Bridge>(),
                       std::make_shared<SpanningTree::Port>() } {}

    BdmSutMachine _sutMachine;
    Mock::Bdm::BeginState _mockBeginState;
    Mock::Bdm::EdgeState _mockEdgeState;
    Mock::Bdm::NotEdgeState _mockNotEdgeState;
};

TEST_F(BridgeDetectionTest,
       testBeginStateExecute_withFailedBridgeBegin_shouldNotChangeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Bdm::BeginState::RealExecute));
    EXPECT_CALL(_sutMachine.BridgeInstance(), Begin()).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kBeginStateName);
}

TEST_F(BridgeDetectionTest,
       testBeginStateExecute_withFailedGoToEdge_shouldChangeStateOntoNotEdgeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Bdm::BeginState::RealExecute));
    EXPECT_CALL(_sutMachine.BridgeInstance(), Begin()).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, GoToEdge(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockBeginState, NotEdgeAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Bdm::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kNotEdgeStateName);
}

TEST_F(BridgeDetectionTest,
       testBeginStateExecute_withSuccessedGoToEdge_shouldChangeStateOntoEdgeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Bdm::BeginState::RealExecute));
    EXPECT_CALL(_sutMachine.BridgeInstance(), Begin()).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, GoToEdge(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, EdgeAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Bdm::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kEdgeStateName);
}

TEST_F(BridgeDetectionTest,
       testEdgeStateExecute_withFailedGoToNotEdge_shouldNotChangeState) {
    EXPECT_CALL(_mockEdgeState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockEdgeState, &Mock::Bdm::EdgeState::RealExecute));
    EXPECT_CALL(_mockEdgeState, GoToNotEdge(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockEdgeState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kEdgeStateName);
}

TEST_F(BridgeDetectionTest,
       testEdgeStateExecute_withSuccessedGoToNotEdge_shouldChangeStateOntoNotEdgeState) {
    EXPECT_CALL(_mockEdgeState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockEdgeState, &Mock::Bdm::EdgeState::RealExecute));
    EXPECT_CALL(_mockEdgeState, GoToNotEdge(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockEdgeState, NotEdgeAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockEdgeState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockEdgeState, &Mock::Bdm::EdgeState::RealChangeState));

    _sutMachine.ChangeState(_mockEdgeState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kNotEdgeStateName);
}

TEST_F(BridgeDetectionTest,
       testNotEdgeStateExecute_withFailedGoToEdge_shouldNotChangeState) {
    EXPECT_CALL(_mockNotEdgeState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockNotEdgeState, &Mock::Bdm::NotEdgeState::RealExecute));
    EXPECT_CALL(_mockNotEdgeState, GoToEdge(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockNotEdgeState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kNotEdgeStateName);
}

TEST_F(BridgeDetectionTest,
       testNotEdgeStateExecute_withSuccessedGoToEdge_shouldChangeStateOntoEdgeState) {
    EXPECT_CALL(_mockNotEdgeState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockNotEdgeState, &Mock::Bdm::NotEdgeState::RealExecute));
    EXPECT_CALL(_mockNotEdgeState, GoToEdge(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockNotEdgeState, EdgeAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockNotEdgeState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockNotEdgeState, &Mock::Bdm::NotEdgeState::RealChangeState));

    _sutMachine.ChangeState(_mockNotEdgeState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 SpanningTree::BridgeDetection::kEdgeStateName);
}