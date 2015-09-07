import QtQuick 2.0
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

Rectangle {

    TestCase {
        id: root
        name: "KingTests"

        Chess.BoardModel {
            id: board
        }

        function init()
        {
            board.clearPieces()
        }

        function test_create()
        {
            var king = board.create("king", 0, 0)
            verify(king, "King is created")
        }

        function test_moves()
        {
            var king = board.create("king", 4, 4)
            verify(king.isValidMove(Qt.point(3, 3)), "Can move up left")
            verify(king.isValidMove(Qt.point(4, 3)), "Can move up")
            verify(king.isValidMove(Qt.point(5, 3)), "Can move up right")

            verify(king.isValidMove(Qt.point(3, 4)), "Can move left")
            verify(king.isValidMove(Qt.point(5, 4)), "Can move right")

            verify(king.isValidMove(Qt.point(3, 5)), "Can move down left")
            verify(king.isValidMove(Qt.point(4, 5)), "Can move down")
            verify(king.isValidMove(Qt.point(5, 5)), "Can move down right")
        }


        function test_doesNotAttackAllies()
        {
            var king = board.create("king", 4, 4)
            board.create("pawn", 4, 3)
            verify(!king.isValidMove(Qt.point(4, 3)), "King will not attack own pawn")
        }


        function test_takes()
        {
            var king = board.create("king", 4, 4, Chess.BoardModel.BLACK)
            board.create("pawn", 4, 3, Chess.BoardModel.WHITE)
            verify(king.isValidMove(Qt.point(4, 3)), "King will attack enemy pawn")
        }
    }
}

