import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import com.luxoft.Chess 1.0 as Chess

Window {
    id: root
    visible: true
    title: "Chess 1.0"
    // property var model: Chess.BoardModel
    width: 500
    height: 500
    property string state: "main"

    Chess.BoardModel {
        id: boardModel
    }

    Chess.BoardView {
        id: boardView
        model: boardModel
        anchors.centerIn: parent
    }

    Item
    {
        id: controlArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: 30

        RowLayout {
            spacing: 6
            anchors.fill: parent

            Button {
                id: startGameButton
                onClicked: startGame()
                text: "Start game"
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "main" || root.state == "replaying"
            }

            Button {
                id: saveGameButton
                text: "Save game"
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "playing"
            }

            Button {
                id: stopGameButton
                text: "Stop game"
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "playing"
                onClicked: stopGame()
            }

            Button {
                id: loadGameButton
                text: "Load game"
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "main" || root.state == "replaying"
            }
        }
    }


    Item
    {
        visible: root.state == "playing"
        id: statusArea
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: 30

        RowLayout {
            spacing: 6
            anchors.fill: parent

            Rectangle
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "lightGrey"
                Text {
                    id: currentPiece
                    text: boardView.selectedPiece ? "Selected: " + boardView.selectedPiece.objectName : "Select piece"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 5
                }
            }

            Rectangle
            {
                Layout.minimumWidth : currentPlayer.contentWidth + 20
                Layout.minimumHeight : currentPlayer.contentHeight
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                color: "lightGrey"
                Text {
                    id: currentPlayer
                    text: boardView.currentPlayer
                    anchors.centerIn: parent
                    anchors.margins: 5
                }
            }
        }
    }

    function setupBoard()
    {
        boardView.resetToDefault()
        // boardModel.clearPieces()
        // boardModel.createPawn(4, 0, Chess.BoardModel.BLACK)
        // boardModel.createPawn(4, 7, Chess.BoardModel.WHITE)
        // boardModel.createPawn(0, 0, Chess.BoardModel.BLACK)
        // boardModel.createPawn(1, 1, Chess.BoardModel.BLACK)
        // boardModel.create("Queen", 6, 4, Chess.BoardModel.WHITE)
        // boardModel.create("Knight", 3, 4, Chess.BoardModel.WHITE)
    }

    function startGame()
    {
        state = "playing"
        boardView.resetToDefault()
    }


    function stopGame()
    {
        state = "main"
        boardModel.clearPieces()
    }

}

