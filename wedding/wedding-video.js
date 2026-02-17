/* ========================================
   婚礼开场视频 - 幻灯片控制器
   ======================================== */

const PROGRESS_UPDATE_INTERVAL_MS = 100;
const MUSIC_VOLUME = 0.7;
const DINNER_MUSIC_VOLUME = 0.2;
const KEY_ESCAPE = 'Escape';
const KEY_SPACE = ' ';

const OSS_BASE =
  'https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/wedding/';

const DINNER_PLAYLIST = [
  `${OSS_BASE}%E4%BB%98%E7%AC%9B%E5%A3%B0%E3%80%81%E4%BB%BB%E9%9D%99%20-%20%E7%9F%A5%E5%BF%83%E7%88%B1%E4%BA%BA.mp3`,
  `${OSS_BASE}%E5%88%81%E5%AF%92-%E8%8A%B1%E5%A5%BD%E6%9C%88%E5%9C%86.mp3`,
  `${OSS_BASE}%E6%B1%9F%E7%8F%8A%20-%20%E6%A2%A6%E9%87%8C%E6%B0%B4%E4%B9%A1.mp3`,
  `${OSS_BASE}%E9%BB%84%E5%AE%89%20-%20%E6%96%B0%E9%B8%B3%E9%B8%AF%E8%9D%B4%E8%9D%B6%E6%A2%A6.mp3`,
];

const initSlideshow = () => {
  const startBtn = document.getElementById('startBtn');
  const slideshow = document.getElementById('slideshow');
  const bgMusic = document.getElementById('bgMusic');
  const dinnerMusic = document.getElementById('dinnerMusic');
  const progressBar = document.getElementById('progressBar');
  const progressFill = document.getElementById('progressFill');
  const slides = Array.from(document.querySelectorAll('.slide'));

  let currentIndex = 0;
  let slideTimer = null;
  let progressTimer = null;
  let presentationStartTime = 0;
  let dinnerTrackIndex = 0;
  let isPlaying = false;

  slides[0].classList.add('active');
  startBtn.style.display = '';

  const coverContent = slides[0].querySelector('.slide-content');
  const coverBg = slides[0].querySelector('.slide-bg');
  const originalCoverHTML = coverContent.innerHTML;

  const getTotalDuration = () =>
    slides.reduce(
      (sum, s) => sum + parseInt(s.dataset.duration || '5000', 10),
      0
    );

  const updateProgress = () => {
    const total = getTotalDuration();
    const elapsed = Date.now() - presentationStartTime;
    const pct = Math.min((elapsed / total) * 100, 100);
    progressFill.style.width = `${pct}%`;
  };

  const showSlide = index => {
    slides.forEach((slide, i) => {
      slide.classList.toggle('active', i === index);
    });
  };

  const addFloatingParticles = () => {
    const count = 15;
    for (let i = 0; i < count; i++) {
      const particle = document.createElement('div');
      particle.className = 'floating-particle';
      particle.style.left = `${Math.random() * 100}%`;
      particle.style.animationDelay = `${Math.random() * 6}s`;
      particle.style.animationDuration = `${4 + Math.random() * 4}s`;
      slideshow.appendChild(particle);
    }
  };

  const playNextDinnerTrack = () => {
    if (dinnerTrackIndex >= DINNER_PLAYLIST.length) {
      dinnerTrackIndex = 0;
    }
    dinnerMusic.src = DINNER_PLAYLIST[dinnerTrackIndex];
    dinnerMusic.volume = DINNER_MUSIC_VOLUME;
    dinnerMusic.play().catch(() => {});
    dinnerTrackIndex++;
  };

  dinnerMusic.addEventListener('ended', playNextDinnerTrack);

  const playDinnerMusic = () => {
    dinnerTrackIndex = 0;
    playNextDinnerTrack();
  };

  const stopPresentation = () => {
    clearTimeout(slideTimer);
    clearInterval(progressTimer);
    isPlaying = false;

    bgMusic.pause();
    bgMusic.currentTime = 0;

    slides.forEach(s => s.classList.remove('active'));
    slides[0].classList.add('active');
    progressBar.classList.add('hidden');
    progressFill.style.width = '0%';

    coverContent.innerHTML = `
      <div class="ornament fade-in-up" style="animation-delay: 0.3s">❦</div>
      <h1 class="title-main fade-in-up" style="animation-delay: 0.8s">请大家吃好喝好！</h1>
    `;
    coverBg.classList.add('blurred');

    playDinnerMusic();
  };

  const scheduleNext = () => {
    const duration = parseInt(
      slides[currentIndex].dataset.duration || '5000',
      10
    );

    slideTimer = setTimeout(() => {
      if (currentIndex < slides.length - 1) {
        currentIndex++;
        showSlide(currentIndex);
        scheduleNext();
      } else {
        stopPresentation();
      }
    }, duration);
  };

  const startPresentation = () => {
    dinnerMusic.pause();
    dinnerMusic.removeAttribute('src');

    coverContent.innerHTML = originalCoverHTML;
    coverBg.classList.remove('blurred');

    startBtn.style.display = 'none';
    progressBar.classList.remove('hidden');

    currentIndex = 0;
    isPlaying = true;
    addFloatingParticles();
    showSlide(0);

    presentationStartTime = Date.now();
    scheduleNext();
    progressTimer = setInterval(updateProgress, PROGRESS_UPDATE_INTERVAL_MS);

    bgMusic.currentTime = 0;
    bgMusic.volume = MUSIC_VOLUME;
    bgMusic.play().catch(() => {});
  };

  startBtn.addEventListener('click', startPresentation);

  document.addEventListener('keydown', e => {
    if (e.key === KEY_ESCAPE && isPlaying) {
      stopPresentation();
    }
    if (e.key === KEY_SPACE) {
      e.preventDefault();
      const activeAudio = isPlaying ? bgMusic : dinnerMusic;
      if (activeAudio.paused) {
        activeAudio.play();
      } else {
        activeAudio.pause();
      }
    }
  });
};

document.addEventListener('DOMContentLoaded', initSlideshow);
