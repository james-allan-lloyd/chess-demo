import QtQuick 2.0

Rectangle {
    id: boardview
    property int cellSize: 40
    property int rows: 8
    property int columns: 8
    property var model: null
    color: "red"  // Debugging size

    Grid {
        id: grid
        anchors.centerIn: parent
        rows: boardview.rows
        columns: boardview.columns
        clip: true
        Repeater {
            model: boardview.model ? boardview.model : boardview.rows * boardview.columns
            Rectangle {
                width: boardview.cellSize
                height: boardview.cellSize
                property int row: Math.floor(index / 8)
                property int col: index % 8
                color: ((row %2) == (col%2)) ? "gray" : "white"
                Text {
                    text: modelData ? modelData.objectName : ""
                    visible: modelData
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

