import QtQuick 2.0
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

TestCase {

    name: "BoardTests"

    Chess.BoardModel {
        id: board
    }

    function test_itStartsWithAnEmptyBoard() {
        compare(board.pieceCount, 0, "New board should not have pieces on it")
    }
}
