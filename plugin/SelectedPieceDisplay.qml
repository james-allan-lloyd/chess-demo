/**
 * Display the select piece and animate it when it moves.
 */
import QtQuick 2.0

Item {
    id: root
    property var boardView: null
    property int moveCellX: 0
    property int moveCellY: 0
    signal moveCompleted(int cellX, int cellY)
    x: boardView.selectedPiece ? boardView.selectedPiece.position.x * boardView.cellSize : 0
    y: boardView.selectedPiece ? boardView.selectedPiece.position.y * boardView.cellSize : 0
    width : boardView.cellSize
    height : boardView.cellSize
    visible: boardView.selectedPiece

    Image {
        source: boardView.selectedPiece ? boardView.selectedPiece.image : ""
        sourceSize.width: boardView.cellSize - 4;
        sourceSize.height: boardView.cellSize - 4;
        anchors.centerIn: parent
    }

    function distance()
    {
        var a = (path.startX - pathEnd.x)
        var b = (path.startY - pathEnd.y)
        return Math.sqrt(a*a + b*b)
    }

    PathAnimation {
        id: pathAnim

        duration: distance() * 5;
        easing.type: Easing.OutQuad
        easing.overshoot: 10

        target: root
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

        onStopped: root.moveCompleted(root.moveCellX, root.moveCellY)
    }

    function moveTo(cellX, cellY)
    {
        root.moveCellX = cellX
        root.moveCellY = cellY
        path.startX = boardView.selectedPiece.position.x * boardView.cellSize
        path.startY = boardView.selectedPiece.position.y * boardView.cellSize
        pathEnd.x = cellX * boardView.cellSize
        pathEnd.y = cellY * boardView.cellSize
        pathAnim.start()
    }

}
