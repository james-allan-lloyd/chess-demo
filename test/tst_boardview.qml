import QtQuick 2.3
import QtQuick.Window 2.2
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

Rectangle {
    id: mainWindow
    visible: true
    width: 500;
    height: 500

    Chess.BoardModel
    {
        id: model
    }

    Chess.BoardView {
        id: view
        model: model
        anchors.centerIn: parent
    }

    TestCase {
        when: windowShown

        function init()
        {
            model.clearPieces()
        }

        function test_createdPieceIsDisplayed()
        {
            model.createPawn(0, 0)
            compare(view.cells.itemAt(0).label, "Pawn")
        }

        function test_movePiece()
        {
            var pawn = model.createPawn(0, 0)
            mouseClick(view.cells.itemAt(0), 5, 5)
            compare(view.selectedPiece, pawn, "Pawn is selected")
            mouseClick(view.cells.itemAt(8), 5, 5)
            verify(pawn.currentPosition === Qt.point(0, 1), "Pawn has correct position")
            compare(view.selectedPiece, null, "Pawn is no longer selected")
        }


        function test_gettingCell()
        {
            verify(view.cell(0, 0))
            verify(view.cell(7, 7))
            verify(!view.cell(8, 8))
            verify(!view.cell(-8, -8))
        }

        // function test_movingPieceToInvalidPosition()
        // {
        //     var pawn = model.createPawn(0, 0)
        //     mouseClick(view.cells.itemAt(0), 5, 5)
        //     compare(view.selectedPiece, pawn, "Pawn is selected")
        //     mouseClick(view.cells.itemAt(8), 5, 5)
        //     verify(pawn.currentPosition === Qt.point(0, 1), "Pawn has correct position")
        //     compare(view.selectedPiece, null, "Pawn is no longer selected")
        // }
    }
}


