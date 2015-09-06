import QtQuick 2.0
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

TestCase {
    id: root
    name: "BoardTests"
    property var board;

    Chess.BoardModel {
        id: board
    }

    function init()
    {
    }

    function cleanup()
    {
        // console.log("cleanup")
        board.clearPieces()
        compare(board.pieceCount, 0)
    }

    function test_itStartsWithAnEmptyBoard() {
        board.clearPieces()
        compare(board.pieceCount, 0, "New board should not have pieces on it")
        for(var i=0; i < board.count; ++i)
        {
            compare(board.get(i), null)
        }
    }

    function test_itCreatesPawns() {
        var pawn = board.createPawn(0, 0)
        verify(pawn, "Creating a pawn shall not return null")
        compare(board.pieceCount, 1, "The piece shall be created on the board")

        board.clearPieces()
        compare(board.pieceCount, 0, "The piece shall be created on the board")
        for(var i=0; i < board.count; ++i)
        {
            compare(board.get(i), null)
        }
    }

    function test_pawnsCanMoveForward() {
        var pawn = board.createPawn(0, 0)
        verify(pawn.isValidMove(Qt.point(0,1)), "Pawn should move forward")
        verify(!pawn.isValidMove(Qt.point(1,1)), "Pawn should not move diagnally without opponent piece")
    }


    function test_pawnsActuallyMovesForward() {
        var pawn = board.createPawn(0, 0)
        verify(pawn.moveTo(Qt.point(0,1)), "Pawn should move forward")
        verify(!pawn.moveTo(Qt.point(1,1)), "Pawn should not move diagnally without opponent piece")
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

    function test_cannotCreatePawnsOutsideBoard()
    {
        verify(!board.createPawn(8,0))
        verify(!board.createPawn(0,8))
        verify(!board.createPawn(-1,0))
        verify(!board.createPawn(0,-1))
    }

    function test_cannotCreatePiecesInOccupiedCells()
    {
        board.createPawn(0, 0)
        verify(!board.createPawn(0, 0))
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
}
