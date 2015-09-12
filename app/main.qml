import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0
import com.luxoft.Chess 1.0 as Chess

Window {
    id: root
    visible: true
    title: "Chess 1.0"
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
                onClicked: saveFileDialog.visible = true
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
                onClicked: loadFileDialog.visible = true
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

    FileDialog {
        id: loadFileDialog
        title: "Load game"
        onAccepted: loadGame(fileUrl)
    }

    FileDialog {
        id: saveFileDialog
        selectExisting: false
        title: "Save game"
        onAccepted: saveGame(fileUrl)
    }

    function setupBoard()
    {
        boardView.resetToDefault()
        // boardModel.clearPieces()
        // boardModel.create("pawn", 4, 0, Chess.BoardModel.BLACK)
        // boardModel.create("pawn", 4, 7, Chess.BoardModel.WHITE)
        // boardModel.create("pawn", 0, 0, Chess.BoardModel.BLACK)
        // boardModel.create("pawn", 1, 1, Chess.BoardModel.BLACK)
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

    function loadGame(filename)
    {
        state = "replaying"
        boardModel.load(filename)
    }


    function saveGame(filename)
    {
        boardModel.save(filename)
    }
}

