import QtQuick 2.0
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

TestCase {
    id: root
    name: "BoardTests"

    Chess.BoardModel {
        id: board
    }

    SignalSpy
    {
        id: dataChangedSpy
        target: board
        signalName: "dataChanged"
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
        var initial = dataChangedSpy.count;
        board.clearPieces()
        compare(board.pieceCount, 0, "New board should not have pieces on it")
        for(var i=0; i < board.count; ++i)
        {
            compare(board.get(i), null)
        }
        verify(dataChangedSpy.count > initial, "Data changed signals were emitted to update the view")
    }

    function test_itCreatesPawns() {
        var pawn = board.createPawn(0, 0)
        verify(pawn, "Creating a pawn shall not return null")
        compare(board.pieceCount, 1, "The piece shall be created on the board")

        board.clearPieces()
        compare(board.pieceCount, 0, "The piece was cleared")
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




    function test_creationEmitsSignal()
    {
        var initialValue = dataChangedSpy.count
        board.createPawn(4,4)
        compare(initialValue + 1, dataChangedSpy.count)
    }


    function test_creatingWhitePieces()
    {
        var whitePawn = board.createPawn(0, 7, Chess.BoardModel.WHITE)
        verify(whitePawn, "Pawn is created successfully")
        compare(whitePawn.color, Chess.BoardModel.WHITE)

        verify(whitePawn.isValidMove(Qt.point(0, 6)), "White pawn moves up the board")
    }


    function test_removingPieces()
    {
        var pawn = board.createPawn(0, 0)
        verify(pawn)
        board.removePiece(pawn)
        compare(board.pieceCount, 0, "Board is empty after removing only piece")
    }


    function test_pawnsTakingEnemyPieces()
    {
        var blackPawn = board.createPawn(0, 0, Chess.BoardModel.BLACK)
        var whitePawn = board.createPawn(1, 1, Chess.BoardModel.WHITE)

        verify(blackPawn.isValidMove(Qt.point(1, 1)), "Black pawn can attack white pawn")
        verify(whitePawn.isValidMove(Qt.point(0, 0)), "White pawn can attack black pawn")

        verify(blackPawn.isValidMove(Qt.point(0, 1)), "Black pawn can still move forward")
        verify(blackPawn.moveTo(Qt.point(1, 1)), "Black pawn takes white pawn")

        compare(board.cell(Qt.point(0,0)), null)

        // NOTE: white pawn is dangling here...
    }


    function test_pawnsCantTakeAllyPieces()
    {
        var blackPawn1 = board.createPawn(0, 0, Chess.BoardModel.BLACK)
        var blackPawn2 = board.createPawn(1, 1, Chess.BoardModel.BLACK)

        verify(!blackPawn1.isValidMove(Qt.point(1, 1)), "Black pawn will not attack other black pawn")
    }
}
