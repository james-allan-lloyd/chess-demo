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

    function test_itStartsWithAnEmptyBoard() {
        board.clearPieces()
        compare(board.pieceCount, 0, "New board should not have pieces on it")
    }

    function test_itCreatesPawns() {
        var pawn = board.createPawn(0, 0)
        verify(pawn, "Creating a pawn shall not return null")
        compare(board.pieceCount, 1, "The piece shall be created on the board")
    }
}
