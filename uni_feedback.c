#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_FEEDBACKS 1000
#define MAX_LENGTH 100
#define MAX_USERS 100

char users[MAX_USERS][MAX_LENGTH];
char passwords[MAX_USERS][MAX_LENGTH];
int userCount = 0;

void loadUsers() {
    FILE *file = fopen("users.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %s", users[userCount], passwords[userCount]) != EOF) {
        userCount++;
    }

    fclose(file);
}

void saveUsers() {
    FILE *file = fopen("users.txt", "w");
    if (!file) {
        perror("Error opening users.txt");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i], passwords[i]);
    }

    fclose(file);
}

void signup() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    char username[MAX_LENGTH], password[MAX_LENGTH];
    printf("Choose username: ");
    fgets(username, MAX_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0;
    if (strlen(username) == 0 || isspace(username[0])) {
        printf("Username cannot be empty or start with a space.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i], username) == 0) {
            printf("Username already exists.\n");
            return;
        }
    }

    printf("Choose password: ");
    fgets(password, MAX_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;
    if (strlen(password) == 0) {
        printf("Password cannot be empty.\n");
        return;
    }

    strcpy(users[userCount], username);
    strcpy(passwords[userCount], password);
    userCount++;

    saveUsers();
    printf("Signup successful. You can now log in.\n");
}

int login() {
    char username[MAX_LENGTH], password[MAX_LENGTH];
    printf("Enter username: ");
    fgets(username, MAX_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, MAX_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i], username) == 0 && strcmp(passwords[i], password) == 0) {
            printf("Login successful. Welcome, %s!\n", username);
            return i;
        }
    }

    printf("Invalid credentials.\n");
    return -1;
}

typedef struct {
    char courseCode[MAX_LENGTH];
    char instructor[MAX_LENGTH];
    int rating;
    char category[MAX_LENGTH];
    char comment[256];
    char author[MAX_LENGTH];
    char date[20];
} Feedback;

Feedback feedbacks[MAX_FEEDBACKS];
int feedbackCount = 0;

void saveFeedbacks() {
    FILE *file = fopen("feedbacks.txt", "w");
    if (!file) {
        perror("Error opening feedbacks.txt");
        return;
    }

    for (int i = 0; i < feedbackCount; i++) {
        fprintf(file, "%s|%s|%d|%s|%s|%s|%s\n",
            feedbacks[i].courseCode, feedbacks[i].instructor, feedbacks[i].rating,
            feedbacks[i].category, feedbacks[i].comment, feedbacks[i].author, feedbacks[i].date);
    }

    fclose(file);
}

void submitFeedback(int userIndex) {
    if (feedbackCount >= MAX_FEEDBACKS) {
        printf("Feedback limit reached.\n");
        return;
    }

    Feedback newFeedback;

    printf("Enter course code: ");
    fgets(newFeedback.courseCode, MAX_LENGTH, stdin);
    newFeedback.courseCode[strcspn(newFeedback.courseCode, "\n")] = 0;

    printf("Enter instructor name: ");
    fgets(newFeedback.instructor, MAX_LENGTH, stdin);
    newFeedback.instructor[strcspn(newFeedback.instructor, "\n")] = 0;

    printf("Enter rating (1-5): ");
    if (scanf("%d", &newFeedback.rating) != 1 || newFeedback.rating < 1 || newFeedback.rating > 5) {
        printf("Invalid rating. Must be between 1 and 5.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("Enter feedback category (e.g., Clarity, Pacing): ");
    fgets(newFeedback.category, MAX_LENGTH, stdin);
    newFeedback.category[strcspn(newFeedback.category, "\n")] = 0;

    printf("Enter comment: ");
    fgets(newFeedback.comment, sizeof(newFeedback.comment), stdin);
    newFeedback.comment[strcspn(newFeedback.comment, "\n")] = 0;

    printf("Enter your name (leave blank for anonymous): ");
    fgets(newFeedback.author, sizeof(newFeedback.author), stdin);
    newFeedback.author[strcspn(newFeedback.author, "\n")] = 0;
    if (strlen(newFeedback.author) == 0) {
        strcpy(newFeedback.author, "Anonymous");
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(newFeedback.date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    feedbacks[feedbackCount++] = newFeedback;
    saveFeedbacks();

    printf("Feedback submitted successfully!\n");
}

void viewAllFeedback() {
    printf("Feedback List:\n");
    if (feedbackCount == 0) {
        printf("No feedback available.\n");
        return;
    }

    for (int i = 0; i < feedbackCount; i++) {
        printf("Course: %s, Instructor: %s, Rating: %d, Category: %s, Comment: %s, Author: %s, Date: %s\n",
            feedbacks[i].courseCode, feedbacks[i].instructor, feedbacks[i].rating,
            feedbacks[i].category, feedbacks[i].comment, feedbacks[i].author, feedbacks[i].date);
    }
}

void exportFeedbackToCSV() {
    FILE *file = fopen("feedbacks.csv", "w");
    if (!file) {
        perror("Error opening feedbacks.csv");
        return;
    }

    if (feedbackCount == 0) {
        printf("No feedback to export.\n");
        fclose(file);
        return;
    }

    fprintf(file, "Course Code,Instructor,Rating,Category,Comment,Author,Date\n");

    for (int i = 0; i < feedbackCount; i++) {
        fprintf(file, "\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\",\"%s\"\n",
            feedbacks[i].courseCode, feedbacks[i].instructor, feedbacks[i].rating,
            feedbacks[i].category, feedbacks[i].comment, feedbacks[i].author, feedbacks[i].date);
    }

    fclose(file);
    printf("Feedback exported to feedbacks.csv.\n");
}

void generateCourseReport() {
    printf("Generating Course Report...\n");

    if (feedbackCount == 0) {
        printf("No feedback available for report.\n");
        return;
    }

    float courseRatings[MAX_FEEDBACKS] = {0};
    int courseFeedbackCount[MAX_FEEDBACKS] = {0};
    char courses[MAX_FEEDBACKS][MAX_LENGTH];
    int uniqueCourses = 0;

    for (int i = 0; i < feedbackCount; i++) {
        int found = -1;
        for (int j = 0; j < uniqueCourses; j++) {
            if (strcmp(feedbacks[i].courseCode, courses[j]) == 0) {
                found = j;
                break;
            }
        }

        if (found == -1) {
            if (uniqueCourses >= MAX_FEEDBACKS) {
                printf("Too many unique courses to process.\n");
                return;
            }
            strcpy(courses[uniqueCourses], feedbacks[i].courseCode);
            found = uniqueCourses;
            uniqueCourses++;
        }

        courseRatings[found] += feedbacks[i].rating;
        courseFeedbackCount[found]++;
    }

    for (int i = 0; i < uniqueCourses; i++) {
        float avgRating = courseRatings[i] / courseFeedbackCount[i];
        printf("Course: %s, Average Rating: %.2f\n", courses[i], avgRating);
    }
}

void generateRatingReport(int minRating) {
    if (minRating < 1 || minRating > 5) {
        printf("Invalid minimum rating. Must be between 1 and 5.\n");
        return;
    }

    printf("Generating Rating Report (Rating >= %d)...\n", minRating);
    int found = 0;

    for (int i = 0; i < feedbackCount; i++) {
        if (feedbacks[i].rating >= minRating) {
            printf("Course: %s, Instructor: %s, Rating: %d, Category: %s, Comment: %s, Author: %s, Date: %s\n",
                feedbacks[i].courseCode, feedbacks[i].instructor, feedbacks[i].rating,
                feedbacks[i].category, feedbacks[i].comment, feedbacks[i].author, feedbacks[i].date);
            found = 1;
        }
    }

    if (!found) {
        printf("No feedback found with rating >= %d.\n", minRating);
    }
}
void editFeedbackByUser(int userIndex) {
    if (feedbackCount == 0) {
        printf("No feedback exists to edit.\n");
        return;
    }

    char username[MAX_LENGTH];
    strcpy(username, users[userIndex]);

    int found = 0;
    printf("Your Feedbacks:\n");

    for (int i = 0; i < feedbackCount; i++) {
        if (strcmp(feedbacks[i].author, username) == 0) {
            printf("ID %d -> Course: %s, Instructor: %s, Rating: %d, Category: %s, Comment: %s, Date: %s\n",
                   i, feedbacks[i].courseCode, feedbacks[i].instructor, feedbacks[i].rating,
                   feedbacks[i].category, feedbacks[i].comment, feedbacks[i].date);
            found = 1;
        }
    }

    if (!found) {
        printf("No feedbacks found under your name.\n");
        return;
    }

    int id;
    printf("Enter the ID of the feedback you want to edit: ");
    if (scanf("%d", &id) != 1 || id < 0 || id >= feedbackCount) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }
    getchar(); // clear newline

    if (strcmp(feedbacks[id].author, username) != 0) {
        printf("You can only edit your own feedback.\n");
        return;
    }

    printf("Editing Feedback ID %d:\n", id);

    printf("Enter new course code (leave blank to keep unchanged): ");
    char input[MAX_LENGTH];
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        strcpy(feedbacks[id].courseCode, input);
    }

    printf("Enter new instructor name (leave blank to keep unchanged): ");
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        strcpy(feedbacks[id].instructor, input);
    }

    printf("Enter new rating (1-5, 0 to keep unchanged): ");
    int newRating;
    if (scanf("%d", &newRating) == 1) {
        if (newRating >= 1 && newRating <= 5) {
            feedbacks[id].rating = newRating;
        } else if (newRating != 0) {
            printf("Invalid rating. Keeping old rating.\n");
        }
    }
    while (getchar() != '\n'); // clear buffer

    printf("Enter new category (leave blank to keep unchanged): ");
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        strcpy(feedbacks[id].category, input);
    }

    printf("Enter new comment (leave blank to keep unchanged): ");
    fgets(input, sizeof(feedbacks[id].comment), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        strcpy(feedbacks[id].comment, input);
    }

    // Update date to today's date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(feedbacks[id].date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    saveFeedbacks();
    printf("Feedback updated successfully!\n");
}


void deleteFeedbackByUser(int userIndex) {
    if (feedbackCount == 0) {
        printf("No feedback exists to delete.\n");
        return;
    }

    char username[MAX_LENGTH];
    strcpy(username, users[userIndex]);

    int found = 0;
    printf("Your Feedbacks:\n");

    for (int i = 0; i < feedbackCount; i++) {
        if (strcmp(feedbacks[i].author, username) == 0) {
            printf("ID %d -> Course: %s, Instructor: %s, Rating: %d, Category: %s, Comment: %s, Date: %s\n",
                   i, feedbacks[i].courseCode, feedbacks[i].instructor, feedbacks[i].rating,
                   feedbacks[i].category, feedbacks[i].comment, feedbacks[i].date);
            found = 1;
        }
    }

    if (!found) {
        printf("No feedbacks found under your name.\n");
        return;
    }

    int deleteIndex;
    printf("Enter the ID of the feedback to delete: ");
    if (scanf("%d", &deleteIndex) != 1) {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n');
        return;
    }

    if (deleteIndex < 0 || deleteIndex >= feedbackCount || strcmp(feedbacks[deleteIndex].author, username) != 0) {
        printf("Invalid ID or not your feedback.\n");
        return;
    }

    printf("Are you sure you want to delete this feedback? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion canceled.\n");
        return;
    }

    for (int i = deleteIndex; i < feedbackCount - 1; i++) {
        feedbacks[i] = feedbacks[i + 1];
    }
    feedbackCount--;

    saveFeedbacks();
    printf("Feedback deleted successfully.\n");
}

void loadFeedbacks() {
    FILE *file = fopen("feedbacks.txt", "r");
    if (!file) return;

    while (fscanf(file, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%19[^\n]\n",
                  feedbacks[feedbackCount].courseCode,
                  feedbacks[feedbackCount].instructor,
                  &feedbacks[feedbackCount].rating,
                  feedbacks[feedbackCount].category,
                  feedbacks[feedbackCount].comment,
                  feedbacks[feedbackCount].author,
                  feedbacks[feedbackCount].date) == 7) {
        feedbackCount++;
    }

    fclose(file);
}

int main() {
    loadUsers();
    loadFeedbacks();

    int userIndex = -1;
    int choice;

    while (1) {
        printf("\nUniversity Course Feedback System\n");
        printf("1. Sign Up\n");
        printf("2. Log In\n");
        printf("3. Submit Feedback\n");
        printf("4. View All Feedback\n");
        printf("5. Generate Course Report\n");
        printf("6. Export Feedback to CSV\n");
        printf("7. Generate Rating Report\n");
        printf("8. Edit  Feedback\n");
        printf("9. Delete  Feedback\n");
        printf("10. Exit\n");
        printf("Choose an option: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                signup();
                break;
            case 2:
                userIndex = login();
                break;
            case 3:
                if (userIndex != -1) {
                    submitFeedback(userIndex);
                } else {
                    printf("Please log in first.\n");
                }
                break;
            case 4:
                viewAllFeedback();
                break;
            case 5:
                generateCourseReport();
                break;
            case 6:
                exportFeedbackToCSV();
                break;
            case 7:
                {
                    int minRating;
                    printf("Enter minimum rating for report: ");
                    if (scanf("%d", &minRating) != 1) {
                        printf("Invalid input.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    generateRatingReport(minRating);
                }
                break;
            case 8:
                 if (userIndex != -1) {
                  editFeedbackByUser(userIndex);
               } else {
                    printf("Please log in first.\n");
                    }
                break;

            case 9:
                if (userIndex != -1) {
                    deleteFeedbackByUser(userIndex);
                } else {
                    printf("Please log in first.\n");
                }
                break;
            case 10:
                printf("Goodbye!\n");
                saveUsers();
                saveFeedbacks();
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
