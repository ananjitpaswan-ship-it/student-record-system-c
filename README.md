# student-record-system-c
"A complete Student Record Management System using C with file handling"
# Create the organized structure
mkdir -p src include docs tests data

# Move your main C file
mv student_record.c src/

# Create README.md
cat > README.md << 'EOF'
# Student Record Management System

A complete C implementation of a Student Record System with file handling capabilities.

## Features
- Add, view, update, delete student records
- Search by ID or name
- Calculate statistics and grade distribution
- Persistent storage using binary files
- Input validation and error handling

## Compilation
```bash
gcc -o student_record src/student_record.c
./student_record
