import QtQuick 2.0

Rectangle {
    id: boardview
    property int cellSize: 40
    property int rows: 8
    property int columns: 8
    property var model: null
    property var cells: cells
    property var selectedPiece: null
    color: "red"  // Debugging size

    Grid {
        // id: grid
        anchors.centerIn: parent
        rows: boardview.rows
        columns: boardview.columns
        clip: true
        Repeater {
            id: cells
            model: boardview.model ? boardview.model : boardview.rows * boardview.columns
            Rectangle {
                width: boardview.cellSize
                height: boardview.cellSize
                property int cellX: index % 8
                property int cellY: Math.floor(index / 8)
                property string label: modelData ? modelData.objectName : ""
                color: ((cellX%2) == (cellY%2)) ? "gray" : "white"
                Text {
                    text: label
                    visible: modelData
                }


                MouseArea {
                    anchors.fill: parent
                    enabled: model != null
                    onClicked: {
                        if (boardview.selectedPiece == null)
                        {
                            if(modelData)
                            {
                                boardview.selectedPiece = modelData
                                // chess.validMoves = selectedPiece.validMoves
                            }
                            else
                            {
                                boardview.selectedPiece = null
                                // chess.validMoves = []
                            }
                        }
                        else
                        {
                            boardview.selectedPiece.moveTo(Qt.point(cellX, cellY));
                            // if(chess.validMoves[0] === index)
                            // {
                            //     // take the piece
                            //     chessModel.movePiece(index, chess.selectedPiece);
                            // }
                            boardview.selectedPiece = null
                            // chess.validMoves = []
                        }
                    }
                }
            }
        }
    }
}

// Item {
//     id: board
//     property var model: null;
//     property int cellSide: 40
//     property int rows: 8
//     property int columns: 8
//
//     Rectangle {
//         anchors.fill: parent
//         color: "blue"
//     }
//
//     Grid {
//         anchors.fill: parent
//         rows: board.rows; columns: board.columns
//         clip: true
//         Repeater {
//             model: board.model
//             Rectangle {
//                 width: board.cellSide; height: board.cellSide
//                 property int row: Math.floor(index / board.columns)
//                 property int col: index % board.columns
//                 color: ((row %2) == (col%2)) ? "gray" : "white"
//             }
//         }
//     }
// }

