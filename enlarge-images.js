function closeModal() {
  document.getElementById("modal-container").style.display = "none";
}

// Open modal, displaying imgElement in full-screen.
function openModal(imgElement) {
  const modalImg = document.getElementById("modal-img");
  modalImg.src = imgElement.src;
  modalImg.alt = imgElement.alt;

  let width, height;
  if (
    imgElement.height / imgElement.width >
    window.innerHeight / window.innerWidth
  ) {
    height = 0.9 * window.innerHeight;
    width = (imgElement.width / imgElement.height) * height;
  } else {
    width = 0.9 * window.innerWidth;
    height = (imgElement.height / imgElement.width) * width;
  }

  modalImg.width = width;
  modalImg.height = height;
  modalImg.style.top = (window.innerHeight - height) / 2 + "px";
  modalImg.style.left = (window.innerWidth - width) / 2 + "px";
  document.getElementById("modal-container").style.display = "block";
}

document.addEventListener("DOMContentLoaded", () => {
  // Close the modal when pressing escape.
  document.addEventListener("keydown", (e) => {
    if (e.key == "Escape") {
      closeModal();
    }
  });

  // Close the modal when clicking anywhere.
  document
    .getElementById("modal-container")
    .addEventListener("click", closeModal);

  // Open the modal if any image is clicked.
  for (const img of document.getElementsByClassName("project-image")) {
    img.addEventListener("click", () => openModal(img));
  }
});
