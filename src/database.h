
typedef struct Evaluation eval;

struct Evaluation {
  char* course_number;
  char* instructor;
  int enrollment;
  double course_quality;
  double difficulty;
  double instructor_quality;
};

char* eval_to_string(eval* e);

void database_init(int init_size, char* filename);
void database_free();

eval** get_all();
eval** get_404();
eval** get_sorted(char* column, char* sorting);
eval** get_filtered(char* column, char* bar);
eval* calculate_average(eval** e);
eval* calculate_min(eval** e);
eval* calculate_max(eval** e);


