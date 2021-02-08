#include <iostream>
#include <map>
#include <vector>
#include <tuple>

using namespace std;

// enum class TaskStatus {
//     NEW,          // новая
//     IN_PROGRESS,  // в разработке
//     TESTING,      // на тестировании
//     DONE          // завершена
// };


// using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
    map <string, TasksInfo> team;
public:
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return team.at(person);
    }
    void AddNewTask(const string& person) {
        team[person][TaskStatus::NEW]++;
    }
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
        const string& person, int task_count) {
        TasksInfo changed, not_changed, result;
        
        if (!team.count(person)) {
            return tie(changed, not_changed);;
        }
        result = team.at(person);
        for (const auto& [key, value] : team[person]) {
            if (key != TaskStatus::DONE) {
                for (int i = 0; i < value; i++) {
                    if (task_count) {
                        int num  = static_cast<int>(key) + 1;

                        result[key]--;
                        if (!result[key]) {
                            result.erase(key);
                        }
                        result[TaskStatus(num)]++;
                        changed[TaskStatus(num)]++;
                        task_count--;
                    }
                    else {
                        not_changed[key]++;
                    }
                }
            }
        }
        team.at(person) = result;
        return tie(changed, not_changed);
    }
};

// void PrintTasksInfo(TasksInfo tasks_info) {
//   cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
//       ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
//       ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
//       ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
// }

// int main() {
//   TeamTasks tasks;
//   tasks.AddNewTask("Ilia");
//   for (int i = 0; i < 3; ++i) {
//     tasks.AddNewTask("Ivan");
//   }
//   cout << "Ilia's tasks: ";
//   PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
//   cout << "Ivan's tasks: ";
//   PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
//   TasksInfo updated_tasks, untouched_tasks;
  
//   tie(updated_tasks, untouched_tasks) =
//       tasks.PerformPersonTasks("Ivan", 2);
//   cout << "Updated Ivan's tasks: ";
//   PrintTasksInfo(updated_tasks);
//   cout << "Untouched Ivan's tasks: ";
//   PrintTasksInfo(untouched_tasks);
  
//   tie(updated_tasks, untouched_tasks) =
//       tasks.PerformPersonTasks("Ivan", 2);
//   cout << "Updated Ivan's tasks: ";
//   PrintTasksInfo(updated_tasks);
//   cout << "Untouched Ivan's tasks: ";
//   PrintTasksInfo(untouched_tasks);

//   return 0;
// }