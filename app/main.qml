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
    property string error: ""

    Chess.BoardModel {
        id: boardModel
    }

    Chess.BoardView {
        id: boardView
        model: boardModel
        anchors.centerIn: parent
        enabled: root.state == "playing" && !winner.length
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
                Layout.minimumWidth: 20
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "main" || root.state == "replaying"
            }

            Button {
                id: saveGameButton
                text: "Save game"
                Layout.minimumWidth: 20
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "playing"
                onClicked: saveFileDialog.visible = true
            }

            Button {
                id: stopGameButton
                text: "Stop game"
                Layout.minimumWidth: 20
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "playing"
                onClicked: stopGame()
            }

            Button {
                id: loadGameButton
                text: "Load game"
                Layout.minimumWidth: 20
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.state == "main" || root.state == "replaying"
                onClicked: loadFileDialog.visible = true
            }

        }
    }


    Item
    {
        visible: root.state == "playing" && boardView.winner.length === 0
        id: statusArea
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: 30

        RowLayout {
            id: statusAreaLayout
            spacing: 6
            anchors.fill: parent

            Rectangle
            {
                Layout.minimumWidth: 20
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

            Button {
                id: undoMoveButton
                text: "Undo"
                Layout.minimumWidth : currentPlayer.contentWidth + 20
                Layout.minimumHeight : currentPlayer.contentHeight
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onClicked: boardView.undo()
                enabled: boardView.canUndo
            }

            Button {
                id: redoMoveButton
                text: "Redo"
                Layout.minimumWidth : currentPlayer.contentWidth + 20
                Layout.minimumHeight : currentPlayer.contentHeight
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onClicked: boardView.redo()
                enabled: boardView.canRedo
            }
        }
    }


    Item
    {
        visible: root.state == "replaying"
        id: replayBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: 30

        RowLayout {
            spacing: 6
            anchors.fill: parent

            Button {
                id: prevMoveButton
                text: "Previous"
                Layout.minimumWidth : currentPlayer.contentWidth + 20
                Layout.minimumHeight : currentPlayer.contentHeight
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: boardView.undo()
                enabled: boardView.canUndo
            }

            Button {
                id: nextMoveButton
                text: "Next"
                Layout.minimumWidth : currentPlayer.contentWidth + 20
                Layout.minimumHeight : currentPlayer.contentHeight
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: boardView.redo()
                enabled: boardView.canRedo
            }
        }
    }


    Rectangle
    {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: 30
        id: errorDisplay
        color: "red"
        visible: error.length > 0
        Text {
            text: error
        }
    }


    Rectangle {
        id: gameOverDisplay
        color: "darkRed"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: 30
        visible: boardView.winner.length > 0 && root.state == "playing"

        Text {
            anchors.centerIn: parent
            text: "Game Over, " + boardView.winner + " wins"
            color: "white"
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
        // boardView.resetToDefault()
        // boardModel.clearPieces()
        // boardModel.create("pawn", 4, 0, Chess.BoardModel.BLACK)
        // boardModel.create("pawn", 4, 7, Chess.BoardModel.WHITE)
        // boardModel.create("pawn", 0, 0, Chess.BoardModel.BLACK)
        // boardModel.create("pawn", 1, 1, Chess.BoardModel.BLACK)
        // boardModel.create("Queen", 6, 4, Chess.BoardModel.WHITE)
        // boardModel.create("Knight", 3, 4, Chess.BoardModel.WHITE)
        boardView.resetTurns()
        boardModel.create("pawn", 3, 3, Chess.BoardModel.BLACK)
        boardModel.create("king", 4, 4, Chess.BoardModel.BLACK)
        boardModel.create("queen", 5, 5, Chess.BoardModel.WHITE)
    }

    function startGame()
    {
        state = "playing"
        boardView.resetToDefault()
        // setupBoard()
        boardView.recorder.restart()
    }


    function stopGame()
    {
        state = "main"
        boardModel.clearPieces()
    }

    function loadGame(filename)
    {
        if(boardView.recorder.load(filename))
        {
            state = "replaying"
            boardView.resetToDefault()
        }
        else
        {
            error = "Failed to load from " + filename
        }
    }


    function saveGame(filename)
    {
        boardView.recorder.save(filename)
    }
}

