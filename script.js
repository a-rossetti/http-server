document.addEventListener("DOMContentLoaded", function() {
    document.body.style.backgroundColor = "#e0e0ff";
    console.log("JavaScript is working!");

    // Change the text color of the paragraph
    const paragraph = document.querySelector("p");
    if (paragraph) {
        paragraph.style.color = "#ff0000"; // Change text color to red
        paragraph.textContent += " This text color was changed by JavaScript.";
    }
});
