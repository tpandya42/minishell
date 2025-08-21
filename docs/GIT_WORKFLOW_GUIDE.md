# GIT_WORKFLOW_GUIDE

## To check branches and find where you are

```bash
git switch branch_name      # or git checkout branch_name but switch is recommended
```

### 1. To see the existent branches

```bash
git branch
```

### 2. To see where you are
```bash
git branch --show-current
```

## Create a branch and move to the new branch
```bash
git checkout -b new_branch_name
```

## Update main from branch (working locally)

### 1. Update the local `main` with the latest changes from GitHub

```bash
git checkout main           # Switch to main
git pull origin main        # Get the latest main version from GitHub
```

---

### 2. Move to your branch

```bash
git switch branch_name      # or git checkout branch_name
```

---

### 3. Sync your branch with the main to test your changes

This is important to avoid errors when pulling or merging later.

If you're in your branch and want to bring changes from `main`, you have two options:

#### 🔄 To bring **all** from `main` (recommended by default)

```bash
git merge main              # Merge all changes from main into your branch
```

#### 🎯 To bring only **specific files** from `main` to the branch where you are (or another branch)

```bash
git checkout main -- file1 file2
```

You can replace `main` with any `other_branch_name`.

---

### 4. Work on your branch

Make your changes. When ready:

```bash
git add .
git commit -m "Your message"
git push origin branch_name
```

> 🔹 To link your branch to the remote on the first push:

```bash
git push -u origin branch_name
```

---

### 5. Move to the local `main`

Now you want to bring specific changes from your branch:

```bash
git switch main
```

---

### 6. Bring what you want from the branch

```bash
git checkout branch_name -- file1 file2
```

⚠️ This is useful only if you **don’t want to merge the whole branch**, just selected files.

---

### 7. Push changes to GitHub's `main`

Make sure your local `main` is ready with git status and then pull

```bash
git status                 # Confirm changes

git add file1 file2
git commit -m "message"
git push origin main
```

### 8. Go to Github to approve Pull request
If there are conflicts most be solved before.
