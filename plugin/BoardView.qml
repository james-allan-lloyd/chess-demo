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
        anchors.centerIn: parent
        rows: boardview.rows
        columns: boardview.columns
        clip: true
        Repeater {
            id: cells
            model: boardview.model ? boardview.model : boardview.rows * boardview.columns
            Rectangle {
                property var validMoveDisplay: validMoveDisplay
                property int cellX: index % 8
                property int cellY: Math.floor(index / 8)
                property string label: modelData ? modelData.objectName : ""
                property color labelColor: (!modelData || modelData.isBlack) ? "black" : "white"
                property color labelText: (!modelData || modelData.isBlack) ? "white" : "black"

                color: ((cellX%2) == (cellY%2)) ? "gray" : "white"
                width: boardview.cellSize
                height: boardview.cellSize

                Rectangle {
                    id: validMoveDisplay
                    anchors.fill: parent
                    color: "yellow"
                    visible: boardview.selectedPiece ? boardview.selectedPiece.isValidMove(Qt.point(cellX, cellY)) : false
                }

                Rectangle {
                    visible: modelData
                    radius: width/2;
                    anchors.fill: parent
                    color: labelText

                    Rectangle {
                        width: parent.width - 4
                        height: parent.height - 4
                        radius: width/2
                        color: labelColor
                        anchors.centerIn: parent
                    }

                    Text {
                        text: label
                        color: labelText
                        anchors.centerIn: parent
                    }
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
                            }
                            else
                            {
                                boardview.selectedPiece = null
                            }
                        }
                        else
                        {
                            boardview.selectedPiece.moveTo(Qt.point(cellX, cellY));
                            boardview.selectedPiece = null
                        }
                    }
                }
            }
        }
    }


    function cell(x, y)
    {
        // Don't need range checking because JS will give you null or undefined anyway.
        return cells.itemAt(x + y * columns)
    }
}
