import streamlit as st

# --- APP CONFIG ---
st.set_page_config(page_title="Student Dashboard", layout="centered")

# --- NAVIGATION ---
menu = st.sidebar.radio("Navigate", ["Home", "Register", "View Students", "Settings"])

# --- PAGE 1: HOME ---
if menu == "Home":
    st.title("🏠 Welcome to Student Dashboard")
    st.write("""
    This simple app demonstrates:
    - Usability: Clear layout and navigation
    - Accessibility: Large fonts and high contrast
    - Consistency: Same theme on every page
    - Feedback: Success messages after actions
    - Error Handling: Graceful handling of invalid inputs
    """)

# --- PAGE 2: REGISTER ---
elif menu == "Register":
    st.title("🧍 Student Registration")
    name = st.text_input("Enter student name:")
    age = st.number_input("Enter age:", min_value=1, max_value=100, step=1)
    grade = st.selectbox("Select grade:", ["A", "B", "C", "D"])
    if st.button("Submit"):
        if name.strip():
            st.success(f"✅ Student '{name}' registered successfully!")
        else:
            st.error("⚠ Please enter a valid name.")

# --- PAGE 3: VIEW STUDENTS ---
elif menu == "View Students":
    st.title("📊 View Students")
    st.info("Sample Data (Can be linked to DB later)")
    data = {
        "Name": ["Alice", "Bob", "Charlie"],
        "Age": [20, 22, 19],
        "Grade": ["A", "B", "A"]
    }
    st.table(data)

# --- PAGE 4: SETTINGS ---
elif menu == "Settings":
    st.title("⚙ Settings")
    theme = st.selectbox("Choose Theme", ["Light", "Dark"])
    st.write(f"Current theme: {theme}")
    st.progress(80)
    st.success("Settings updated successfully!")