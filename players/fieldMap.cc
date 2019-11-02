#include "fieldMap.hh"
#include <unordered_set>

CellInfo **cells;

void initFieldMap(GameInfo &info) {
  cells = new CellInfo*[info.size];
  for (int x = 0; x != info.size; x++) {
    cells[x] = new CellInfo[info.size];
  }
  static const int dx[] = { 0,-1,-1,-1, 0, 1, 1, 1 };
  static const int dy[] = { 1, 1, 0,-1,-1,-1, 0, 1 };
  for (int x = 0; x != info.size; x++) {
    for (int y = 0; y != info.size; y++) {
      CellInfo &c = cells[x][y];
      c.position = Cell(x, y);
      for (int k = 0; k != 8; k++) {
		int nx = x + dx[k];
		int ny = y + dy[k];
		if (0 <= nx && nx < info.size &&
			0 <= ny && ny < info.size) {
			c.eightNeighbors.push_back(&cells[nx][ny]);
			if (k%2 == 0) c.fourNeighbors.push_back(&cells[nx][ny]);
		}
	  }
	}
  }
}

/*
samuraiDistance:CellInfo型のfromからtoまでの侍の最短距離を計算する
起点から未探索、かつ穴がないマスを再帰的に探索していく

*/
int samuraiDistance
(CellInfo *from, CellInfo *to, set <Cell> &holes) {
  stack <CellInfo*> next0; //np0にはdist回で到達しうる場所の一覧(CellInfoの配列)が入る
  stack <CellInfo*> next1; //np0にはdist+1回で到達しうる場所の一覧(CellInfoの配列)が入る
  stack <CellInfo*> next2; //np0にはdist+2回で到達しうる場所の一覧(CellInfoの配列)が入る
  next0.push(from);
  stack <CellInfo*> *np0 = &next0;
  stack <CellInfo*> *np1 = &next1;
  stack <CellInfo*> *np2 = &next2;
  unordered_set <CellInfo*> visited;
  for (int dist = 0; ; dist++) { //現在のマスからdist回で到達可能なマスを探していく
    while (!np0->empty()) { //dist-1回で到達可能な場所について以下の処理を実施する
      CellInfo *c = np0->top(); np0->pop(); //np0の先頭を取り出してcに入れる
		if (visited.count(c) == 0) { //すでに到達済みなら無視する(最短距離ではないから)
		visited.insert(c);//到達済みリストに追加する
		for (auto n: c->fourNeighbors) { //cの隣接4マスを順番にnに入れていく(Python のfor n in c.fourneighborsと同じ)
			if (n == to) return dist;//探索終了
			if (holes.count(n->position) == 0) {
				np1->push(n); //穴が開いていないならば次回到達可能なのでnp1に代入
			} else {
				np2->push(n); //穴があいているならば穴埋めに一回消費するのでnp2に代入
			}
		}
      }
    }
    np0 = np1; np1 = np2; np2 = np0;//dist回で到達可能なマスを調べ終わったので、次回の探索対象として一つずつ時間をずらす(np2=np0はダミー)
  }
}
