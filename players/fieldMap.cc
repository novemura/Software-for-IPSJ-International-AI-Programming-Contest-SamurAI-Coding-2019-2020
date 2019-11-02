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
samuraiDistance:CellInfo�^��from����to�܂ł̎��̍ŒZ�������v�Z����
�N�_���疢�T���A�������Ȃ��}�X���ċA�I�ɒT�����Ă���

*/
int samuraiDistance
(CellInfo *from, CellInfo *to, set <Cell> &holes) {
  stack <CellInfo*> next0; //np0�ɂ�dist��œ��B������ꏊ�̈ꗗ(CellInfo�̔z��)������
  stack <CellInfo*> next1; //np0�ɂ�dist+1��œ��B������ꏊ�̈ꗗ(CellInfo�̔z��)������
  stack <CellInfo*> next2; //np0�ɂ�dist+2��œ��B������ꏊ�̈ꗗ(CellInfo�̔z��)������
  next0.push(from);
  stack <CellInfo*> *np0 = &next0;
  stack <CellInfo*> *np1 = &next1;
  stack <CellInfo*> *np2 = &next2;
  unordered_set <CellInfo*> visited;
  for (int dist = 0; ; dist++) { //���݂̃}�X����dist��œ��B�\�ȃ}�X��T���Ă���
    while (!np0->empty()) { //dist-1��œ��B�\�ȏꏊ�ɂ��Ĉȉ��̏��������{����
      CellInfo *c = np0->top(); np0->pop(); //np0�̐擪�����o����c�ɓ����
		if (visited.count(c) == 0) { //���łɓ��B�ς݂Ȃ疳������(�ŒZ�����ł͂Ȃ�����)
		visited.insert(c);//���B�ς݃��X�g�ɒǉ�����
		for (auto n: c->fourNeighbors) { //c�̗א�4�}�X�����Ԃ�n�ɓ���Ă���(Python ��for n in c.fourneighbors�Ɠ���)
			if (n == to) return dist;//�T���I��
			if (holes.count(n->position) == 0) {
				np1->push(n); //�����J���Ă��Ȃ��Ȃ�Ύ��񓞒B�\�Ȃ̂�np1�ɑ��
			} else {
				np2->push(n); //���������Ă���Ȃ�Ό����߂Ɉ������̂�np2�ɑ��
			}
		}
      }
    }
    np0 = np1; np1 = np2; np2 = np0;//dist��œ��B�\�ȃ}�X�𒲂׏I������̂ŁA����̒T���ΏۂƂ��Ĉ�����Ԃ����炷(np2=np0�̓_�~�[)
  }
}
