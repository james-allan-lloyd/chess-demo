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
            view.resetTurns()
        }

        function test_createdPieceIsDisplayed()
        {
            model.createPawn(0, 0)
            compare(view.cells.itemAt(0).label, "Pawn")
        }

        function comparePoint(actual, expected)
        {
            compare(actual.x, expected.x)
            compare(actual.y, expected.y)
        }

        function test_movePiece()
        {
            var pawn = model.createPawn(0, 7, Chess.BoardModel.WHITE)
            // mouseClick(view.cell(0, 0))
            mouseClick(view.cell(0, 7), 5, 5)
            compare(view.selectedPiece, pawn, "Pawn is selected")
            mouseClick(view.cell(0, 6), 5, 5)
            comparePoint(pawn.currentPosition, Qt.point(0, 6), "Pawn has correct position")
            compare(view.selectedPiece, null, "Pawn is no longer selected")
        }


        function test_gettingCell()
        {
            verify(view.cell(0, 0))
            verify(view.cell(7, 7))
            verify(!view.cell(8, 8))
            verify(!view.cell(-8, -8))
        }

        function test_movingPieceToInvalidPosition()
        {
            var invalidPosition = Qt.point(7, 7)
            var pawn = model.createPawn(0, 0, Chess.BoardModel.WHITE)
            mouseClick(view.cell(0, 0), 5, 5)
            compare(view.selectedPiece, pawn, "Pawn is selected")
            mouseClick(view.cell(invalidPosition.x, invalidPosition.y), 5, 5)
            comparePoint(pawn.currentPosition, Qt.point(0,0), "Pawn has not moved")
        }


        function test_validMovesAreDisplayed()
        {
            var pawn = model.createPawn(0, 7, Chess.BoardModel.WHITE)
            mouseClick(view.cell(0, 7), 5, 5)

            verify(pawn.isValidMove(Qt.point(0,6)), "Pawn can move 1")
            verify(pawn.isValidMove(Qt.point(0,5)), "Pawn can move 2 (on first move)")

            verify(view.cell(0,6).validMoveDisplay.visible)
            verify(view.cell(0,5).validMoveDisplay.visible)
        }


        function test_pieceColorsAreDisplayed()
        {
            var blackPawn = model.createPawn(0,0, Chess.BoardModel.BLACK)
            var whitePawn = model.createPawn(0,7, Chess.BoardModel.WHITE)

            verify(Qt.colorEqual(view.cell(0,0).labelColor, "black"))
            verify(Qt.colorEqual(view.cell(0,7).labelColor, "white"))
        }


        function test_whiteStartsFirst()
        {
            compare(view.currentPlayer, "white");
        }

        function test_movingOtherPlayersPiecesNotAllowed()
        {
            compare(view.currentPlayer, "white");

            var blackPawn = model.createPawn(0,0, Chess.BoardModel.BLACK)
            var whitePawn = model.createPawn(0,7, Chess.BoardModel.WHITE)

            mouseClick(view.cell(0, 0), 5, 5)
            compare(view.selectedPiece, null)

            mouseClick(view.cell(0, 7), 5, 5)
            compare(view.selectedPiece, whitePawn)
        }


        function test_movingPieceEndsTurn()
        {
            compare(view.currentPlayer, "white");

            var blackPawn = model.createPawn(0,0, Chess.BoardModel.BLACK)
            var whitePawn = model.createPawn(0,7, Chess.BoardModel.WHITE)

            mouseClick(view.cell(0,7), 5, 5)
            compare(view.selectedPiece, whitePawn)
            mouseClick(view.cell(0,6), 5, 5)
            comparePoint(whitePawn.currentPosition, Qt.point(0,6))

            compare(view.currentPlayer, "black");
            mouseClick(view.cell(0,0), 5, 5)
            compare(view.selectedPiece, blackPawn)
            mouseClick(view.cell(0,0), 5, 5) // invalid move
            compare(view.currentPlayer, "black");

            mouseClick(view.cell(0,0), 5, 5)
            compare(view.selectedPiece, blackPawn)
            mouseClick(view.cell(0,1), 5, 5) // valid move
            comparePoint(blackPawn.currentPosition, Qt.point(0,1))

            compare(view.currentPlayer, "white");
        }
    }
}


