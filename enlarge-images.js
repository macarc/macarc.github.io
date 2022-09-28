document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll("img").forEach((img) => {
    if (img.id !== "modal-img") {
      img.addEventListener("click", () => {
        const modalImg = document.getElementById("modal-img");
        modalImg.src = img.src;
        modalImg.alt = img.alt;
        let width, height;
        if (
          img.height / img.width >
          window.innerHeight / window.innerWidth
        ) {
          height = 0.9 * window.innerHeight;
          width = (img.width / img.height) * height;
        } else {
          width = 0.9 * window.innerWidth;
          height = (img.height / img.width) * width;
        }
        modalImg.width = width;
        modalImg.height = height;
        modalImg.style.top = (window.innerHeight - height) / 2 + "px";
        modalImg.style.left = (window.innerWidth - width) / 2 + "px";
        document.getElementById("img-modal").style.display = "block";
      });
    }
  });
});
