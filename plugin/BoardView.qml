import QtQuick 2.0
import com.luxoft.Chess 1.0 as Chess


Rectangle {
    id: boardview
    property int cellSize: 40
    property int rows: 8
    property int columns: 8
    property var model: null
    property var cells: cells
    property var selectedPiece: null
    property string currentPlayer: "white"
    property var recorder: recorder
    property alias canUndo: recorder.canUndo
    property alias canRedo: recorder.canRedo


    color: "red"  // Debugging size

    Chess.Recorder {
        id: recorder
        model: boardview.model
    }

    Grid {
        anchors.centerIn: parent
        rows: boardview.rows
        columns: boardview.columns
        clip: true
        Repeater {
            id: cells
            property int selectedIndex: -1;
            model: boardview.model ? boardview.model : boardview.rows * boardview.columns
            Rectangle {
                property var validMoveDisplay: validMoveDisplay
                property int cellX: index % 8
                property int cellY: Math.floor(index / 8)
                property string label: modelData ? modelData.objectName : ""
                property string pieceColor: (!modelData || modelData.isBlack) ? "black" : "white"
                property color labelColor: (!modelData || modelData.isBlack) ? "black" : "white"
                property color labelText: (!modelData || modelData.isBlack) ? "white" : "black"

                color: ((cellX%2) == (cellY%2)) ? "lightGray" : "white"
                width: boardview.cellSize
                height: boardview.cellSize

                Rectangle {
                    id: validMoveDisplay
                    anchors.fill: parent
                    color: "#20FFFF00"
                    visible: boardview.selectedPiece ? boardview.selectedPiece.isValidMove(Qt.point(cellX, cellY)) : false
                }

                Rectangle {
                    id: selectablePieceDisplay
                    anchors.fill: parent
                    color: "#200000FF"
                    visible: !boardview.selectedPiece && modelData && boardview.currentPlayer == pieceColor
                }

                // Rectangle {
                //     visible: modelData
                //     radius: width/2;
                //     anchors.fill: parent
                //     color: labelText

                //     Rectangle {
                //         width: parent.width - 4
                //         height: parent.height - 4
                //         radius: width/2
                //         color: labelColor
                //         anchors.centerIn: parent
                //     }

                //     Text {
                //         text: label
                //         color: labelText
                //         anchors.centerIn: parent
                //     }
                // }

                Image {
                    source: modelData ? modelData.image : ""
                    sourceSize.width: cellSize - 4;
                    sourceSize.height: cellSize - 4;
                    visible: modelData && modelData != boardview.selectedPiece
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: model != null
                    onClicked: {
                        if (boardview.selectedPiece == null)
                        {
                            if(modelData)
                            {
                                if((modelData.isBlack && currentPlayer == "black")
                                  || (modelData.isWhite && currentPlayer == "white"))
                                {
                                    boardview.selectedPiece = modelData
                                    animTester.x = parent.cellX * boardview.cellSize
                                    animTester.y = parent.cellY * boardview.cellSize
                                }
                            }
                        }
                        else
                        {
                            if(boardview.selectedPiece.isValidMove(Qt.point(cellX, cellY)))
                            {
                                pathAnim.startMove(parent.cellX, parent.cellY)
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: animTester
            width : cellSize
            height : cellSize
            x: 0
            y: 0
            visible: boardview.selectedPiece
            Image {
                source: boardview.selectedPiece ? boardview.selectedPiece.image : ""
                sourceSize.width: cellSize - 4;
                sourceSize.height: cellSize - 4;
                anchors.centerIn: parent
            }
        }

        PathAnimation {
            id: pathAnim

            duration: 500
            easing.type: Easing.InQuad

            target: animTester
            path: Path {
                id: path
                startX: 50
                startY: 50

                PathLine {
                    id: pathEnd
                    x: 0
                    y: 0
                }
            }

            onStopped: completeMove()

            property int moveCellX: 0
            property int moveCellY: 0

            function startMove(cellX, cellY)
            {
                console.log("Move to " + String(cellX) + ", " + String(cellY))
                moveCellX = cellX
                moveCellY = cellY
                path.startX = animTester.x
                path.startY = animTester.y
                pathEnd.x = cellX * boardview.cellSize
                pathEnd.y = cellY * boardview.cellSize
                pathAnim.start()
            }

            function completeMove()
            {
                recorder.move(boardview.selectedPiece, moveCellX, moveCellY)
                if(currentPlayer == "white")
                {
                    currentPlayer = "black"
                }
                else
                {
                    currentPlayer = "white"
                }

                boardview.selectedPiece = null
            }
        }

        // ParallelAnimation {
        //     id: animTesterAnim
        //     property int toX: 0
        //     property int toY: 0
        //     PropertyAnimation {
        //         target: animTester
        //         property: "x"
        //         to: parent.toX
        //         duration: 500
        //     }
        //     PropertyAnimation {
        //         target: animTester
        //         property: "y"
        //         to: parent.toY
        //         duration: 500
        //     }
        // }
    }




    function cell(x, y)
    {
        // Don't need range checking because JS will give you null or undefined anyway.
        return cells.itemAt(x + y * columns)
    }


    function resetTurns()
    {
        currentPlayer = "white"
    }


    function undo()
    {
        if(recorder.undo())
        {
            currentPlayer = currentPlayer == "black" ? "white" : "black"
        }
    }


    function redo()
    {
        if(recorder.redo())
        {
            currentPlayer = currentPlayer == "black" ? "white" : "black"
        }
    }


    function resetToDefault()
    {
        resetTurns()
        model.clearPieces()
        model.create("Rook",   0, 0, Chess.BoardModel.BLACK)
        model.create("Knight", 1, 0, Chess.BoardModel.BLACK)
        model.create("Bishop", 2, 0, Chess.BoardModel.BLACK)
        model.create("Queen",  3, 0, Chess.BoardModel.BLACK)
        model.create("King",   4, 0, Chess.BoardModel.BLACK)
        model.create("Bishop", 5, 0, Chess.BoardModel.BLACK)
        model.create("Knight", 6, 0, Chess.BoardModel.BLACK)
        model.create("Rook",   7, 0, Chess.BoardModel.BLACK)

        for(var i=0; i<8; ++i)
        {
            model.create("Pawn", i, 1, Chess.BoardModel.BLACK)
        }

        for(var i=0; i<8; ++i)
        {
            model.create("Pawn", i, 6, Chess.BoardModel.WHITE)
        }

        model.create("Rook",   0, 7, Chess.BoardModel.WHITE)
        model.create("Knight", 1, 7, Chess.BoardModel.WHITE)
        model.create("Bishop", 2, 7, Chess.BoardModel.WHITE)
        model.create("King",   3, 7, Chess.BoardModel.WHITE)
        model.create("Queen",  4, 7, Chess.BoardModel.WHITE)
        model.create("Bishop", 5, 7, Chess.BoardModel.WHITE)
        model.create("Knight", 6, 7, Chess.BoardModel.WHITE)
        model.create("Rook",   7, 7, Chess.BoardModel.WHITE)
    }
}
