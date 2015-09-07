import QtQuick 2.0
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

Rectangle {

    TestCase {
        id: root
        name: "PawnTests"

        Chess.BoardModel {
            id: board
        }

        function init()
        {
            // console.log("cleanup")
            board.clearPieces()
            compare(board.pieceCount, 0)
        }

        function cleanup()
        {
        }


        function test_cantMoveThroughOwnPieces()
        {
            var pawn = board.createPawn(0, 7, Chess.BoardModel.WHITE)
            var pawnInTheWay = board.createPawn(0, 6, Chess.BoardModel.WHITE)

            verify(!pawn.isValidMove(pawnInTheWay.currentPosition))
            verify(!pawn.isValidMove(Qt.point(0, 5))) // should also ensure we can't jump the pawn
        }


        function test_pawnsActuallyMovesForward() {
            var pawn = board.createPawn(0, 0)
            verify(pawn.moveTo(Qt.point(0,1)), "Pawn should move forward")
            verify(!pawn.moveTo(Qt.point(1,1)), "Pawn should not move diagonally without opponent piece")
            compare(pawn.currentPosition, Qt.point(0,1), "Pawn is in the correct position")
        }


        function test_pawnDoesNotMoveOffBoard()
        {
            var pawn = board.createPawn(0, 7)
            verify(!pawn.isValidMove(Qt.point(0,8)))
            verify(!pawn.moveTo(Qt.point(0,8)))
            compare(pawn.currentPosition, Qt.point(0,7))

            var pawn = board.createPawn(0, 0)
            verify(!pawn.moveTo(pawn, Qt.point(0,-1)))
        }


        function test_pawnCanMoveTwoOnFirstTurnButNotAfter()
        {
            var pawn = board.createPawn(0, 0)
            verify(pawn.isValidMove(Qt.point(0,2)))
            verify(pawn.moveTo(Qt.point(0,2)));
            compare(pawn.currentPosition, Qt.point(0,2));

            verify(pawn.isValidMove(Qt.point(0,3)), "Pawn should be able to move one after first move");
            verify(!pawn.isValidMove(Qt.point(0,4)), "Pawn should not be able to move 2 after first move");
        }


        function test_pawnDoesNotCaptureForward()
        {
            var pawn = board.createPawn(0, 0, Chess.BoardModel.BLACK)
            var enemyPawn = board.createPawn(0, 1, Chess.BoardModel.WHITE)

            verify(!pawn.isValidMove(Qt.point(0, 1)))
            verify(!pawn.isValidMove(Qt.point(0, 2)))

            var pawn2 = board.createPawn(1, 0, Chess.BoardModel.BLACK)
            var enemyPawn2 = board.createPawn(1, 2, Chess.BoardModel.WHITE)
            verify(pawn2.isValidMove(Qt.point(1,1)))
            verify(!pawn2.isValidMove(Qt.point(1,2)))
        }
    }
}
