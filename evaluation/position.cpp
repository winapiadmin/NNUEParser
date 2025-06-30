// assume library=disservin/chess-library
#include "position.h"
namespace NNUEParser{
  Position::Position(const _Board& board){
		b      = board;
		npm[1] = piece_value(chess::PieceType::KNIGHT)
				 * b.pieces(chess::PieceType::KNIGHT, chess::Color::BLACK).count()
			   + piece_value(chess::PieceType::BISHOP)
				   * b.pieces(chess::PieceType::BISHOP, chess::Color::BLACK).count()
			   + piece_value(chess::PieceType::ROOK)
				   * b.pieces(chess::PieceType::ROOK, chess::Color::BLACK).count()
			   + piece_value(chess::PieceType::QUEEN)
				   * b.pieces(chess::PieceType::QUEEN, chess::Color::BLACK).count();
		npm[0] = piece_value(chess::PieceType::KNIGHT)
				 * b.pieces(chess::PieceType::KNIGHT, chess::Color::WHITE).count()
			   + piece_value(chess::PieceType::BISHOP)
				   * b.pieces(chess::PieceType::BISHOP, chess::Color::WHITE).count()
			   + piece_value(chess::PieceType::ROOK)
				   * b.pieces(chess::PieceType::ROOK, chess::Color::WHITE).count()
			   + piece_value(chess::PieceType::QUEEN)
				   * b.pieces(chess::PieceType::QUEEN, chess::Color::WHITE).count();
  }
  inline Bitboard Position::pieces(Color c) const {
      return b.us(c).getBits();
  }
  inline Bitboard Position::pieces(Color c, PieceType pt) const {
      switch (pt)
      {
      case ALL_PIECES :
          return pieces(c);
      default :
          return b.pieces(chess::PieceType(static_cast<chess::PieceType::underlying>(pt - 1)), c)
            .getBits();
      }
  }
  inline Bitboard Position::pieces(PieceType pt) const {
      switch (pt)
      {
      case ALL_PIECES :
          return b.occ().getBits();
      default :
          return b.pieces(chess::PieceType(static_cast<chess::PieceType::underlying>(pt - 1)))
            .getBits();
      }
  }
  inline Bitboard Position::pieces() const { return b.occ().getBits(); }
  template<PieceType Pt>
  inline int Position::count() const {
      return chess::Bitboard(pieces(Pt)).count();
  }
  template<PieceType Pt>
  inline int Position::count(Color c) const {
      return chess::Bitboard(pieces(c, Pt)).count();
  }
  inline Color Position::side_to_move() const { return (Color) static_cast<int>(b.sideToMove()); }
  inline Piece Position::piece_on(const Square sq) const { return to_engine_piece(b.at(sq).internal()); }
  inline Value Position::non_pawn_material() const { return npm[0] + npm[1]; }
  inline Value Position::non_pawn_material(Color c) const { return npm[(int) c]; }
  inline int   Position::rule50_count() const { return b.halfMoveClock(); }
  DirtyPiece   Position::make_move(_Move m) {
      DirtyPiece dp;
      dp.pc          = to_engine_piece(b.at(m.from()).internal());
      dp.from        = (Square) m.from().index();
      dp.to          = (Square) m.to().index();
      dp.add_sq      = SQ_NONE;
      Color us       = side_to_move();
      Color them     = ~us;
      Piece captured = m.typeOf() == m.ENPASSANT ? make_piece(them, PAWN) : piece_on(dp.to);
      if (m.typeOf() == m.CASTLING)
      {
          const bool king_side = m.to() > m.from();
          const auto rookTo    = chess::Square::castling_rook_square(king_side, side_to_move());
          const auto kingTo    = chess::Square::castling_king_square(king_side, side_to_move());
          dp.to                = (Square) kingTo.index();
          dp.remove_pc = dp.add_pc = make_piece(us, ROOK);
          dp.remove_sq             = (Square) m.to().index();
          dp.add_sq                = (Square) rookTo.index();
          captured                 = NO_PIECE;
      }
      else if (captured)
      {
          Square capsq =
            m.typeOf() == m.ENPASSANT ? Square(m.to().index() - (us == WHITE ? 8 : -8)) : dp.to;
          dp.remove_pc = captured;
          dp.remove_sq = capsq;
      }
      else
          dp.remove_sq = SQ_NONE;
      if (m.typeOf() == m.PROMOTION)
      {
          dp.add_pc =
            make_piece(us,
                         static_cast<PieceType>(
                         1 + m.promotionType()));
          dp.add_sq = dp.to;
          dp.to     = SQ_NONE;  // promotion moves are not allowed to have a to square
      }
      // Non pawn material
      if (m.typeOf() == m.PROMOTION)
      {
          npm[(int) us] += piece_value(m.promotionType());
      }
      // en passant and PxP are handled in piece_value
      if (b.isCapture(m))
          npm[(int) them] -= piece_value(b.at<chess::PieceType>(m.to()));

      b.makeMove(m);  // finally no
      return dp;
  }
  void Position::undo_move(_Move m) {
      b.unmakeMove(m);
      Color us   = side_to_move();
      Color them = ~side_to_move();
      if (m.typeOf() == m.PROMOTION)
      {
          npm[(int) us] -= piece_value(m.promotionType());
      }
      // en passant and PxP are handled in piece_value
      if (b.isCapture(m))
          npm[(int) them] += piece_value(b.at<chess::PieceType>(m.to()));
  }
  void         Position::do_null_move() { b.makeNullMove(); }
  void         Position::undo_null_move() { b.unmakeNullMove(); }
}
