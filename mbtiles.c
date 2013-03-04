#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "mbtiles.h"

struct MBTilesIterator {
  sqlite3 *sqlConn;
  sqlite3_stmt *sqlStmt;
  int sqlStmtResult;
};

int mbtiles_iterator_new(MBTilesIterator **self_ptr, MBTilesMetadata *metadata, const char *path) {
  *self_ptr = (MBTilesIterator*) malloc(sizeof(MBTilesIterator));
  (*self_ptr)->sqlConn = 0;
  (*self_ptr)->sqlStmt = 0;
  (*self_ptr)->sqlStmtResult = 0;
  int result = sqlite3_open(path, &((*self_ptr)->sqlConn));
  if (result != SQLITE_OK) {
     return -1;
  }
  char sqlQuery[] = "SELECT zoom_level, tile_column, tile_row, tile_data FROM tiles;";
  result = sqlite3_prepare((*self_ptr)->sqlConn, sqlQuery, sizeof(sqlQuery), &((*self_ptr)->sqlStmt), 0);
  if (result != SQLITE_OK) {
     return -2;
  }
  (*self_ptr)->sqlStmtResult = sqlite3_step((*self_ptr)->sqlStmt);
  return 0;
}

void mbtiles_iterator_free(MBTilesIterator *self) {
  if (self->sqlConn != NULL) {
    sqlite3_close(self->sqlConn);
  }
  if (self->sqlStmt != NULL) {
    sqlite3_finalize(self->sqlStmt);
  }
  if (self != NULL) {
    free(self);
  }
}

int mbtiles_iterator_finished(const MBTilesIterator *self) {
  return (self->sqlStmtResult == SQLITE_DONE);
}

int mbtiles_iterator_get(MBTilesIterator *self, MBTile *tile) {
  if (self->sqlStmtResult != SQLITE_ROW) {
    return -1;
  }
  tile->zoom_level = sqlite3_column_int(self->sqlStmt, 0);
  tile->column = sqlite3_column_int(self->sqlStmt, 1);
  tile->row = sqlite3_column_int(self->sqlStmt, 2);
  tile->data = (const char *) sqlite3_column_blob(self->sqlStmt, 3);
  tile->data_size = sqlite3_column_bytes(self->sqlStmt, 3);
  self->sqlStmtResult = sqlite3_step(self->sqlStmt);
}

#define HAVE_MAIN 1

#if HAVE_MAIN
int main(int argc, char **argv) {
  MBTilesIterator *iter;
  MBTilesMetadata metadata;
  MBTile tile;
  int res = mbtiles_iterator_new(&iter, &metadata, argv[1]);
  if (res == 0) {
    while (!mbtiles_iterator_finished(iter)) {
      mbtiles_iterator_get(iter, &tile);
      printf("%i %i %i\n", tile.zoom_level, tile.column, tile.row);
    }
    mbtiles_iterator_free(iter);
  } else {
    printf("error");
    return 1;
  }
  return 0;
}
#endif
