#include "global.h"
#include "berry.h"
#include "event_object_movement.h"
#include "field_control_avatar.h"
#include "fieldmap.h"
#include "item.h"
#include "item_menu.h"
#include "item_use.h"
#include "main.h"
#include "menu.h"
#include "random.h"
#include "task.h"
#include "text.h"
#include "constants/berry.h"
#include "constants/event_object_movement.h"
#include "constants/items.h"

#define NAME_CHERI_BERRY   _("버치")
#define NAME_CHESTO_BERRY  _("유루")
#define NAME_PECHA_BERRY   _("복슝")
#define NAME_RAWST_BERRY   _("복분")
#define NAME_ASPEAR_BERRY  _("배리")
#define NAME_LEPPA_BERRY   _("과사")
#define NAME_ORAN_BERRY    _("오랭")
#define NAME_PERSIM_BERRY  _("시몬")
#define NAME_LUM_BERRY     _("리샘")
#define NAME_SITRUS_BERRY  _("자뭉")
#define NAME_FIGY_BERRY    _("무화")
#define NAME_WIKI_BERRY    _("위키")
#define NAME_MAGO_BERRY    _("마고")
#define NAME_AGUAV_BERRY   _("아바")
#define NAME_IAPAPA_BERRY  _("파야")
#define NAME_RAZZ_BERRY    _("라즈")
#define NAME_BLUK_BERRY    _("블리")
#define NAME_NANAB_BERRY   _("나나")
#define NAME_WEPEAR_BERRY  _("서배")
#define NAME_PINAP_BERRY   _("파인")
#define NAME_POMEG_BERRY   _("유석")
#define NAME_KELPSY_BERRY  _("시마")
#define NAME_QUALOT_BERRY  _("파비")
#define NAME_HONDEW_BERRY  _("로매")
#define NAME_GREPA_BERRY   _("또뽀")
#define NAME_TAMATO_BERRY  _("토망")
#define NAME_CORNN_BERRY   _("수숙")
#define NAME_MAGOST_BERRY  _("고스티")
#define NAME_RABUTA_BERRY  _("라부탐")
#define NAME_NOMEL_BERRY   _("노멜")
#define NAME_SPELON_BERRY  _("메호키")
#define NAME_PAMTRE_BERRY  _("자야")
#define NAME_WATMEL_BERRY  _("슈박")
#define NAME_DURIN_BERRY   _("듀리")
#define NAME_BELUE_BERRY   _("루베")
#define NAME_LIECHI_BERRY  _("치리")
#define NAME_GANLON_BERRY  _("용아")
#define NAME_SALAC_BERRY   _("캄라")
#define NAME_PETAYA_BERRY  _("야타비")
#define NAME_APICOT_BERRY  _("규살")
#define NAME_LANSAT_BERRY  _("랑사")
#define NAME_STARF_BERRY   _("스타")
#define NAME_ENIGMA_BERRY  _("의문")

static const u8 sBerryDescriptionPart1_Cheri[] = _("매우 귀여운 꽃이 핀다.");
static const u8 sBerryDescriptionPart2_Cheri[] = _("새빨간 열매는 아주 맵다.");
static const u8 sBerryDescriptionPart1_Chesto[] = _("겉 부분과 속 모두");
static const u8 sBerryDescriptionPart2_Chesto[] = _("딱딱하다. 어느 부분을 먹어도 떫다.");
static const u8 sBerryDescriptionPart1_Pecha[] = _("매우 달고 맛있다.");
static const u8 sBerryDescriptionPart2_Pecha[] = _("부드러워서 운반할 때는 주의.");
static const u8 sBerryDescriptionPart1_Rawst[] = _("잎사귀의 부분이 길고 둥글게");
static const u8 sBerryDescriptionPart2_Rawst[] = _("말려 있으면 아주 쓴 듯하다.");
static const u8 sBerryDescriptionPart1_Aspear[] = _("딱딱한 열매 속에는 수분이 매우");
static const u8 sBerryDescriptionPart2_Aspear[] = _("많이 들어 있다. 꽤 시다.");
static const u8 sBerryDescriptionPart1_Leppa[] = _("버치 등과 비교해 느리게 자란다.");
static const u8 sBerryDescriptionPart2_Leppa[] = _("열매가 작은 것일수록 맛있다.");
static const u8 sBerryDescriptionPart1_Oran[] = _("여러 가지 맛이 섞인 신기한 열매.");
static const u8 sBerryDescriptionPart2_Oran[] = _("열매를 키우는 데 반나절이 걸린다.");
static const u8 sBerryDescriptionPart1_Persim[] = _("태양을 좋아한다. 햇빛을 받으면");
static const u8 sBerryDescriptionPart2_Persim[] = _("점점 색이 선명해진다.");
static const u8 sBerryDescriptionPart1_Lum[] = _("자라는 데 시간이 걸리지만 소중하게");
static const u8 sBerryDescriptionPart2_Lum[] = _("키우면 열매가 2개 자라기도 한다.");
static const u8 sBerryDescriptionPart1_Sitrus[] = _("오랭과는 형제 같은 존재.");
static const u8 sBerryDescriptionPart2_Sitrus[] = _("크고 맛도 고르다.");
static const u8 sBerryDescriptionPart1_Figy[] = _("먹힌 듯한 모양 속에는");
static const u8 sBerryDescriptionPart2_Figy[] = _("매운 성분이 가득 차 있다.");
static const u8 sBerryDescriptionPart1_Wiki[] = _("주위에 붙어 있는 돌기는 포켓몬이");
static const u8 sBerryDescriptionPart2_Wiki[] = _("지니기 쉽게 하기 위해서라고 한다.");
static const u8 sBerryDescriptionPart1_Mago[] = _("크게 자랄수록 열매가 구부러진다.");
static const u8 sBerryDescriptionPart2_Mago[] = _("구부러질수록 달고 맛있다.");
static const u8 sBerryDescriptionPart1_Aguav[] = _("작은 크기의 꽃으로 그것도 빛을");
static const u8 sBerryDescriptionPart2_Aguav[] = _("필요로 하지 않는 희귀한 나무열매다.");
static const u8 sBerryDescriptionPart1_Iapapa[] = _("매우 크고 신 나무열매다.");
static const u8 sBerryDescriptionPart2_Iapapa[] = _("키우는 데는 적어도 1일이 걸린다.");
static const u8 sBerryDescriptionPart1_Razz[] = _("빨간 열매는 먹으면 약간 맵고");
static const u8 sBerryDescriptionPart2_Razz[] = _("빠르게 자라서 4시간이면 열매가 열린다.");
static const u8 sBerryDescriptionPart1_Bluk[] = _("겉 부분은 파란색이지만 먹으면");
static const u8 sBerryDescriptionPart2_Bluk[] = _("입속이 새까맣게 된다.");
static const u8 sBerryDescriptionPart1_Nanab[] = _("세상에서 7번째로 발견된 것이");
static const u8 sBerryDescriptionPart2_Nanab[] = _("이름의 유래라는 설이 유력하다. 달다.");
static const u8 sBerryDescriptionPart1_Wepear[] = _("작고 하얀 꽃이 핀다. 쓴맛과");
static const u8 sBerryDescriptionPart2_Wepear[] = _("신맛이 섞여 있는 미묘한 맛이다.");
static const u8 sBerryDescriptionPart1_Pinap[] = _("바람에 약하고 추위에도 약하다.");
static const u8 sBerryDescriptionPart2_Pinap[] = _("열매의 속이 맵고 겉 부분은 시다.");
static const u8 sBerryDescriptionPart1_Pomeg[] = _("아무리 비가 내려도 영양이");
static const u8 sBerryDescriptionPart2_Pomeg[] = _("있어도 6개의 열매밖에 자라지 않는다.");
static const u8 sBerryDescriptionPart1_Kelpsy[] = _("뿌리 같은 모양의 희귀한");
static const u8 sBerryDescriptionPart2_Kelpsy[] = _("종류이다. 매우 큰 꽃이 핀다.");
static const u8 sBerryDescriptionPart1_Qualot[] = _("물을 좋아한다. 계속 비가 내리는");
static const u8 sBerryDescriptionPart2_Qualot[] = _("장소라도 쑥쑥 잘 자란다.");
static const u8 sBerryDescriptionPart1_Hondew[] = _("매우 비싸서 좀처럼 눈으로");
static const u8 sBerryDescriptionPart2_Hondew[] = _("보기 힘든 나무열매. 아주 맛있다.");
static const u8 sBerryDescriptionPart1_Grepa[] = _("부드러움과 둥근 모양으로는");
static const u8 sBerryDescriptionPart2_Grepa[] = _("상상할 수 없을 정도로 시다.");
static const u8 sBerryDescriptionPart1_Tamato[] = _("입술이 새빨개질 정도로 맵다.");
static const u8 sBerryDescriptionPart2_Tamato[] = _("키우는 데에도 시간이 걸린다.");
static const u8 sBerryDescriptionPart1_Cornn[] = _("옛날부터 자라왔다. 많이 심지");
static const u8 sBerryDescriptionPart2_Cornn[] = _("않으면 열매가 열리지 않을 때도 있다.");
static const u8 sBerryDescriptionPart1_Magost[] = _("매우 맛의 균형이 좋다고");
static const u8 sBerryDescriptionPart2_Magost[] = _("평판이 자자한 나무열매다.");
static const u8 sBerryDescriptionPart1_Rabuta[] = _("많은 털이 자라있는 희귀한");
static const u8 sBerryDescriptionPart2_Rabuta[] = _("종류이다. 꽤 쓰다.");
static const u8 sBerryDescriptionPart1_Nomel[] = _("꽤 시다. 한입 먹으면");
static const u8 sBerryDescriptionPart2_Nomel[] = _("3일간 다른 맛이 나지 않게 된다.");
static const u8 sBerryDescriptionPart1_Spelon[] = _("새빨개진 열매는 말도 못하게 맵다.");
static const u8 sBerryDescriptionPart2_Spelon[] = _("돌기로부터도 매운 성분이 나오고 있다.");
static const u8 sBerryDescriptionPart1_Pamtre[] = _("어딘가에서 난데없이 떠내려온다.");
static const u8 sBerryDescriptionPart2_Pamtre[] = _("세상 어딘가에서 키워지고 있는 듯하다.");
static const u8 sBerryDescriptionPart1_Watmel[] = _("매우 커서 50cm를 넘는 것도");
static const u8 sBerryDescriptionPart2_Watmel[] = _("발견되었다. 굉장히 달다");
static const u8 sBerryDescriptionPart1_Durin[] = _("보기만 해도 쓰다. 너무나도 써서");
static const u8 sBerryDescriptionPart2_Durin[] = _("그대로 먹은 사람은 아무도 없다.");
static const u8 sBerryDescriptionPart1_Belue[] = _("매끈해서 맛있어 보이지만 말도 못하게");
static const u8 sBerryDescriptionPart2_Belue[] = _("시다. 키우는 데에도 시간이 걸린다.");
static const u8 sBerryDescriptionPart1_Liechi[] = _("수수께끼의 씨앗. 치리열매에는");
static const u8 sBerryDescriptionPart2_Liechi[] = _("바다의 힘이 깃들어 있다는 소문이 있다.");
static const u8 sBerryDescriptionPart1_Ganlon[] = _("수수께끼의 씨앗. 용아열매에는");
static const u8 sBerryDescriptionPart2_Ganlon[] = _("육지의 힘이 깃들어 있다는 소문이 있다.");
static const u8 sBerryDescriptionPart1_Salac[] = _("수수께끼의 씨앗. 캄라열매에는");
static const u8 sBerryDescriptionPart2_Salac[] = _("하늘의 힘이 깃들어 있다는 소문이 있다.");
static const u8 sBerryDescriptionPart1_Petaya[] = _("수수께끼의 씨앗. 야타비열매에는 모든");
static const u8 sBerryDescriptionPart2_Petaya[] = _("생명체의 힘이 깃들어 있다고 전해진다.");
static const u8 sBerryDescriptionPart1_Apicot[] = _("이상하고 이상한 나무열매다. 무엇이");
static const u8 sBerryDescriptionPart2_Apicot[] = _("일어날지 무엇이 생길지 알 수 없다.");
static const u8 sBerryDescriptionPart1_Lansat[] = _("전설의 열매라고 전해진다. 이 열매를");
static const u8 sBerryDescriptionPart2_Lansat[] = _("가지고 있으면 매우 행복해지는 듯하다.");
static const u8 sBerryDescriptionPart1_Starf[] = _("너무나도 힘이 강하기 때문에");
static const u8 sBerryDescriptionPart2_Starf[] = _("세상의 끝에 버려진 나무열매다.");
static const u8 sBerryDescriptionPart1_Enigma[] = _("정체불명의 나무열매다.");
static const u8 sBerryDescriptionPart2_Enigma[] = _("별의 힘을 지니고 있는 듯하다.");

const struct Berry gBerries[] =
{
    {
        .name = NAME_CHERI_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 20,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Cheri,
        .description2 = sBerryDescriptionPart2_Cheri,
        .stageDuration = 3,
        .spicy = 10,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_CHESTO_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 80,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Chesto,
        .description2 = sBerryDescriptionPart2_Chesto,
        .stageDuration = 3,
        .spicy = 0,
        .dry = 10,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_PECHA_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_SOFT,
        .size = 40,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Pecha,
        .description2 = sBerryDescriptionPart2_Pecha,
        .stageDuration = 3,
        .spicy = 0,
        .dry = 0,
        .sweet = 10,
        .bitter = 0,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_RAWST_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 32,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Rawst,
        .description2 = sBerryDescriptionPart2_Rawst,
        .stageDuration = 3,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 10,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_ASPEAR_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 50,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Aspear,
        .description2 = sBerryDescriptionPart2_Aspear,
        .stageDuration = 3,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 10,
        .smoothness = 25,
    },
    {
        .name = NAME_LEPPA_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 28,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Leppa,
        .description2 = sBerryDescriptionPart2_Leppa,
        .stageDuration = 4,
        .spicy = 10,
        .dry = 0,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_ORAN_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 35,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Oran,
        .description2 = sBerryDescriptionPart2_Oran,
        .stageDuration = 3,
        .spicy = 10,
        .dry = 10,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_PERSIM_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 47,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Persim,
        .description2 = sBerryDescriptionPart2_Persim,
        .stageDuration = 3,
        .spicy = 10,
        .dry = 10,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_LUM_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 34,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Lum,
        .description2 = sBerryDescriptionPart2_Lum,
        .stageDuration = 12,
        .spicy = 10,
        .dry = 10,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_SITRUS_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 95,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Sitrus,
        .description2 = sBerryDescriptionPart2_Sitrus,
        .stageDuration = 6,
        .spicy = 10,
        .dry = 10,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_FIGY_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 100,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Figy,
        .description2 = sBerryDescriptionPart2_Figy,
        .stageDuration = 6,
        .spicy = 10,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_WIKI_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 115,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Wiki,
        .description2 = sBerryDescriptionPart2_Wiki,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 10,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_MAGO_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 126,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Mago,
        .description2 = sBerryDescriptionPart2_Mago,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 0,
        .sweet = 10,
        .bitter = 0,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_AGUAV_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 64,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Aguav,
        .description2 = sBerryDescriptionPart2_Aguav,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 10,
        .sour = 0,
        .smoothness = 25,
    },
    {
        .name = NAME_IAPAPA_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 223,
        .maxYield = 3,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Iapapa,
        .description2 = sBerryDescriptionPart2_Iapapa,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 10,
        .smoothness = 25,
    },
    {
        .name = NAME_RAZZ_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 120,
        .maxYield = 6,
        .minYield = 3,
        .description1 = sBerryDescriptionPart1_Razz,
        .description2 = sBerryDescriptionPart2_Razz,
        .stageDuration = 1,
        .spicy = 10,
        .dry = 10,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 20,
    },
    {
        .name = NAME_BLUK_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 108,
        .maxYield = 6,
        .minYield = 3,
        .description1 = sBerryDescriptionPart1_Bluk,
        .description2 = sBerryDescriptionPart2_Bluk,
        .stageDuration = 1,
        .spicy = 0,
        .dry = 10,
        .sweet = 10,
        .bitter = 0,
        .sour = 0,
        .smoothness = 20,
    },
    {
        .name = NAME_NANAB_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 77,
        .maxYield = 6,
        .minYield = 3,
        .description1 = sBerryDescriptionPart1_Nanab,
        .description2 = sBerryDescriptionPart2_Nanab,
        .stageDuration = 1,
        .spicy = 0,
        .dry = 0,
        .sweet = 10,
        .bitter = 10,
        .sour = 0,
        .smoothness = 20,
    },
    {
        .name = NAME_WEPEAR_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 74,
        .maxYield = 6,
        .minYield = 3,
        .description1 = sBerryDescriptionPart1_Wepear,
        .description2 = sBerryDescriptionPart2_Wepear,
        .stageDuration = 1,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_PINAP_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 80,
        .maxYield = 6,
        .minYield = 3,
        .description1 = sBerryDescriptionPart1_Pinap,
        .description2 = sBerryDescriptionPart2_Pinap,
        .stageDuration = 1,
        .spicy = 10,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_POMEG_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 135,
        .maxYield = 6,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Pomeg,
        .description2 = sBerryDescriptionPart2_Pomeg,
        .stageDuration = 3,
        .spicy = 10,
        .dry = 0,
        .sweet = 10,
        .bitter = 10,
        .sour = 0,
        .smoothness = 20,
    },
    {
        .name = NAME_KELPSY_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 150,
        .maxYield = 6,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Kelpsy,
        .description2 = sBerryDescriptionPart2_Kelpsy,
        .stageDuration = 3,
        .spicy = 0,
        .dry = 10,
        .sweet = 0,
        .bitter = 10,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_QUALOT_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 110,
        .maxYield = 6,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Qualot,
        .description2 = sBerryDescriptionPart2_Qualot,
        .stageDuration = 3,
        .spicy = 10,
        .dry = 0,
        .sweet = 10,
        .bitter = 0,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_HONDEW_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 162,
        .maxYield = 6,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Hondew,
        .description2 = sBerryDescriptionPart2_Hondew,
        .stageDuration = 3,
        .spicy = 10,
        .dry = 10,
        .sweet = 0,
        .bitter = 10,
        .sour = 0,
        .smoothness = 20,
    },
    {
        .name = NAME_GREPA_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 149,
        .maxYield = 6,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Grepa,
        .description2 = sBerryDescriptionPart2_Grepa,
        .stageDuration = 3,
        .spicy = 0,
        .dry = 10,
        .sweet = 10,
        .bitter = 0,
        .sour = 10,
        .smoothness = 20,
    },
    {
        .name = NAME_TAMATO_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 200,
        .maxYield = 4,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Tamato,
        .description2 = sBerryDescriptionPart2_Tamato,
        .stageDuration = 6,
        .spicy = 20,
        .dry = 10,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 30,
    },
    {
        .name = NAME_CORNN_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 75,
        .maxYield = 4,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Cornn,
        .description2 = sBerryDescriptionPart2_Cornn,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 20,
        .sweet = 10,
        .bitter = 0,
        .sour = 0,
        .smoothness = 30,
    },
    {
        .name = NAME_MAGOST_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 140,
        .maxYield = 4,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Magost,
        .description2 = sBerryDescriptionPart2_Magost,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 0,
        .sweet = 20,
        .bitter = 10,
        .sour = 0,
        .smoothness = 30,
    },
    {
        .name = NAME_RABUTA_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 226,
        .maxYield = 4,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Rabuta,
        .description2 = sBerryDescriptionPart2_Rabuta,
        .stageDuration = 6,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 20,
        .sour = 10,
        .smoothness = 30,
    },
    {
        .name = NAME_NOMEL_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 285,
        .maxYield = 4,
        .minYield = 2,
        .description1 = sBerryDescriptionPart1_Nomel,
        .description2 = sBerryDescriptionPart2_Nomel,
        .stageDuration = 6,
        .spicy = 10,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 20,
        .smoothness = 30,
    },
    {
        .name = NAME_SPELON_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 133,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Spelon,
        .description2 = sBerryDescriptionPart2_Spelon,
        .stageDuration = 18,
        .spicy = 40,
        .dry = 10,
        .sweet = 0,
        .bitter = 0,
        .sour = 0,
        .smoothness = 70,
    },
    {
        .name = NAME_PAMTRE_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_SOFT,
        .size = 244,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Pamtre,
        .description2 = sBerryDescriptionPart2_Pamtre,
        .stageDuration = 18,
        .spicy = 0,
        .dry = 40,
        .sweet = 10,
        .bitter = 0,
        .sour = 0,
        .smoothness = 70,
    },
    {
        .name = NAME_WATMEL_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 250,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Watmel,
        .description2 = sBerryDescriptionPart2_Watmel,
        .stageDuration = 18,
        .spicy = 0,
        .dry = 0,
        .sweet = 40,
        .bitter = 10,
        .sour = 0,
        .smoothness = 70,
    },
    {
        .name = NAME_DURIN_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 280,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Durin,
        .description2 = sBerryDescriptionPart2_Durin,
        .stageDuration = 18,
        .spicy = 0,
        .dry = 0,
        .sweet = 0,
        .bitter = 40,
        .sour = 10,
        .smoothness = 70,
    },
    {
        .name = NAME_BELUE_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_SOFT,
        .size = 300,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Belue,
        .description2 = sBerryDescriptionPart2_Belue,
        .stageDuration = 18,
        .spicy = 10,
        .dry = 0,
        .sweet = 0,
        .bitter = 0,
        .sour = 40,
        .smoothness = 70,
    },
    {
        .name = NAME_LIECHI_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 111,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Liechi,
        .description2 = sBerryDescriptionPart2_Liechi,
        .stageDuration = 24,
        .spicy = 40,
        .dry = 0,
        .sweet = 40,
        .bitter = 0,
        .sour = 10,
        .smoothness = 80,
    },
    {
        .name = NAME_GANLON_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 33,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Ganlon,
        .description2 = sBerryDescriptionPart2_Ganlon,
        .stageDuration = 24,
        .spicy = 0,
        .dry = 40,
        .sweet = 0,
        .bitter = 40,
        .sour = 0,
        .smoothness = 80,
    },
    {
        .name = NAME_SALAC_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 95,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Salac,
        .description2 = sBerryDescriptionPart2_Salac,
        .stageDuration = 24,
        .spicy = 0,
        .dry = 0,
        .sweet = 40,
        .bitter = 0,
        .sour = 40,
        .smoothness = 80,
    },
    {
        .name = NAME_PETAYA_BERRY,
        .firmness = BERRY_FIRMNESS_VERY_HARD,
        .size = 237,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Petaya,
        .description2 = sBerryDescriptionPart2_Petaya,
        .stageDuration = 24,
        .spicy = 40,
        .dry = 0,
        .sweet = 0,
        .bitter = 40,
        .sour = 0,
        .smoothness = 80,
    },
    {
        .name = NAME_APICOT_BERRY,
        .firmness = BERRY_FIRMNESS_HARD,
        .size = 75,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Apicot,
        .description2 = sBerryDescriptionPart2_Apicot,
        .stageDuration = 24,
        .spicy = 0,
        .dry = 40,
        .sweet = 0,
        .bitter = 0,
        .sour = 40,
        .smoothness = 80,
    },
    {
        .name = NAME_LANSAT_BERRY,
        .firmness = BERRY_FIRMNESS_SOFT,
        .size = 97,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Lansat,
        .description2 = sBerryDescriptionPart2_Lansat,
        .stageDuration = 24,
        .spicy = 10,
        .dry = 10,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 30,
    },
    {
        .name = NAME_STARF_BERRY,
        .firmness = BERRY_FIRMNESS_SUPER_HARD,
        .size = 153,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Starf,
        .description2 = sBerryDescriptionPart2_Starf,
        .stageDuration = 24,
        .spicy = 10,
        .dry = 10,
        .sweet = 10,
        .bitter = 10,
        .sour = 10,
        .smoothness = 30,
    },
    {
        .name = NAME_ENIGMA_BERRY,
        .firmness = BERRY_FIRMNESS_UNKNOWN,
        .size = 0,
        .maxYield = 2,
        .minYield = 1,
        .description1 = sBerryDescriptionPart1_Enigma,
        .description2 = sBerryDescriptionPart2_Enigma,
        .stageDuration = 24,
        .spicy = 40,
        .dry = 40,
        .sweet = 40,
        .bitter = 40,
        .sour = 40,
        .smoothness = 40,
    },
};

static const struct BerryTree gBlankBerryTree = {0};

extern u8 S_BerryTree[];
extern u16 gSpecialVar_LastTalked;
extern u16 gSpecialVar_0x8004;
extern u16 gSpecialVar_0x8005;
extern u16 gSpecialVar_0x8006;

/*
    An enigma berry is a type of berry which functions
    as though it is a custom berry. Because it behaves
    like its a custom berry, it doesn't hold an identity
    represented in its icons or descriptions, leaving its
    information to be decided by the e-reader cards
    which were intended to deliver these custom
    berries.
*/

static u8 CalcBerryYield(struct BerryTree *tree);
static u16 GetStageDurationByBerryType(u8 berry);

#if DEBUG

extern u8 sub_80B47D8(u16 var);

u8 debug_sub_80C2B04(void)
{
    u8 taskId = sub_80B47D8(0);

    gTasks[taskId].data[4]--;
    CloseMenu();
    return 1;
}

u8 debug_sub_80C2B30(void)
{
    u8 taskId = sub_80B47D8(1);

    gTasks[taskId].data[4]--;
    CloseMenu();
    return 1;
}

#endif

// unused
// this could be static, but making it so causes a compile-time warning.
void ClearEnigmaBerries(void)
{
    CpuFill16(0, &gSaveBlock1.enigmaBerry, sizeof(gSaveBlock1.enigmaBerry));
}

void SetEnigmaBerry(u8 *src)
{
    // initialize the enigma berry by copying the data from the script.
    u32 i;
    u8 *dest = (u8*)&gSaveBlock1.enigmaBerry;

    for (i = 0; i < sizeof(gSaveBlock1.enigmaBerry); i++)
        dest[i] = src[i];

    // at this point, the description pointer is not yet initialized. we need to initialize it since
    // we dont know where in memory this is going to be. set the berry desc pointers to the
    // EnigmaBerry struct's description arrays since these are where the descriptions are stored.
    gSaveBlock1.enigmaBerry.berry.description1 = gSaveBlock1.enigmaBerry.description1;
    gSaveBlock1.enigmaBerry.berry.description2 = gSaveBlock1.enigmaBerry.description2;
}

static u32 GetEnigmaBerryChecksum(struct EnigmaBerry *enigmaBerry)
{
    const u8 *description1;
    const u8 *description2;
    u32 i;
    u32 checksum;
    u8 *dest;

    // the description pointers could be pointing to anywhere in memory. we do not want these
    // pointers to factor into the checksum as it will produce a different result every time: so
    // back the pointers up and set them to null so the checksum is safe to calculate.
    description1 = gSaveBlock1.enigmaBerry.berry.description1;
    description2 = gSaveBlock1.enigmaBerry.berry.description2;
    gSaveBlock1.enigmaBerry.berry.description1 = NULL;
    gSaveBlock1.enigmaBerry.berry.description2 = NULL;

    dest = (u8*)enigmaBerry;
    checksum = 0;
    for (i = 0; i < ((u32)&gSaveBlock1.enigmaBerry.checksum - (u32)&gSaveBlock1.enigmaBerry); i++)
        checksum += dest[i];

    // the checksum is calculated: the descriptions are safe to restore now.
    gSaveBlock1.enigmaBerry.berry.description1 = description1;
    gSaveBlock1.enigmaBerry.berry.description2 = description2;

    return checksum;
}

#if DEBUG

extern const u8 gSpriteImage_UnusedCherry[];
extern const u16 gSpritePalette_UnusedCherry[];
extern u8 gUnknown_Debug_839B6CE[];

static const u8 gUnknown_Debug_083F7F84[] = DTR("そとから　きた　きのみ", "외부에서 온 나무열매");
static const u8 gUnknown_Debug_083F7F90[] = DTR("ただいま　かいはつちゅう", "현재 개발 중");

void debug_sub_80C2C18(const u8 *name, u8 holdEffect, u8 holdEffectParam)
{
    s32 i;

    gSaveBlock1.enigmaBerry.berry = gBerries[0];
    StringCopy(gSaveBlock1.enigmaBerry.berry.name, name);
    StringCopy(gSaveBlock1.enigmaBerry.description1, gUnknown_Debug_083F7F84);
    StringCopy(gSaveBlock1.enigmaBerry.description2, gUnknown_Debug_083F7F90);
    gSaveBlock1.enigmaBerry.berry.description1 = gSaveBlock1.enigmaBerry.description1;
    gSaveBlock1.enigmaBerry.berry.description2 = gSaveBlock1.enigmaBerry.description2;
    for (i = 0; i < 0x480; i++)
        gSaveBlock1.enigmaBerry.pic[i] = gSpriteImage_UnusedCherry[i];
    for (i = 0; i < 16; i++)
        gSaveBlock1.enigmaBerry.palette[i] = gSpritePalette_UnusedCherry[i];
    for (i = 0; i < 18; i++)
        gSaveBlock1.enigmaBerry.itemEffect[i] = gUnknown_Debug_839B6CE[i];
    gSaveBlock1.enigmaBerry.holdEffect = holdEffect;
    gSaveBlock1.enigmaBerry.holdEffectParam = holdEffectParam;
    gSaveBlock1.enigmaBerry.checksum = GetEnigmaBerryChecksum(&gSaveBlock1.enigmaBerry);
}

void debug_sub_80C2D24(u8 spicy, u8 dry, u8 sweet, u8 bitter, u8 sour, u8 smoothness)
{
    gSaveBlock1.enigmaBerry.berry.spicy = spicy;
    gSaveBlock1.enigmaBerry.berry.dry = dry;
    gSaveBlock1.enigmaBerry.berry.sweet = sweet;
    gSaveBlock1.enigmaBerry.berry.bitter = bitter;
    gSaveBlock1.enigmaBerry.berry.sour = sour;
    gSaveBlock1.enigmaBerry.berry.smoothness = smoothness;
    gSaveBlock1.enigmaBerry.checksum = GetEnigmaBerryChecksum(&gSaveBlock1.enigmaBerry);
}

#endif

// due to e-reader scans being particularly volatile to failure, it is a requirement to check for
// their integrity here due to scans possibly failing to produce the correct result.
bool32 IsEnigmaBerryValid(void)
{
    if (gSaveBlock1.enigmaBerry.berry.stageDuration == 0)
        return FALSE;
    if (gSaveBlock1.enigmaBerry.berry.maxYield == 0)
        return FALSE;
    if (GetEnigmaBerryChecksum(&gSaveBlock1.enigmaBerry) != gSaveBlock1.enigmaBerry.checksum)
        return FALSE;
    return TRUE;
}

const struct Berry *GetBerryInfo(u8 berry)
{
    // when getting the pointer to the berry info, enigma berries are handled differently. if your
    // berry is an Enigma Berry and its checksum is valid, fetch the pointer to its information in
    // the save block.
    if (berry == ITEM_TO_BERRY(ITEM_ENIGMA_BERRY) && IsEnigmaBerryValid())
        return &gSaveBlock1.enigmaBerry.berry;
    else
    {
        // invalid berries will be flattened into a cheri berry. Interestingly, if your berry was
        // an enigma berry whos checksum failed, the game will use the Enigma Berry information
        // for this: meaning if you see the Enigma Berry information, its actually because the
        // checksum failed.
        if (berry == BERRY_NONE || berry > ITEM_TO_BERRY(LAST_BERRY_INDEX))
            berry = ITEM_TO_BERRY(FIRST_BERRY_INDEX);
        return &gBerries[berry - 1];
    }
}

// the save file can handle up to a number of 128 berry trees as indicated by its definition
// in global.h. Interestingly, this function does not check that limit of 128.
static struct BerryTree *GetBerryTreeInfo(u8 id)
{
    return &gSaveBlock1.berryTrees[id];
}

// this was called because the berry script was successful: meaning the player chose to
// water the tree. We need to check for the current tree stage and set the appropriate
// water flag to true.
bool32 ObjectEventInteractionWaterBerryTree(void)
{
    // GetBerryTreeInfo does not sanitize the tree retrieved, but there are no known
    // instances where this can cause problems.
    struct BerryTree *tree = GetBerryTreeInfo(ObjectEventGetBerryTreeId(gSelectedObjectEvent));

    switch (tree->stage)
    {
    case BERRY_STAGE_PLANTED:
        tree->watered1 = TRUE;
        break;
    case BERRY_STAGE_SPROUTED:
        tree->watered2 = TRUE;
        break;
    case BERRY_STAGE_TALLER:
        tree->watered3 = TRUE;
        break;
    case BERRY_STAGE_FLOWERING:
        tree->watered4 = TRUE;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

bool8 IsPlayerFacingUnplantedSoil(void)
{
    if (GetObjectEventScriptPointerPlayerFacing() == S_BerryTree
     && GetStageByBerryTreeId(ObjectEventGetBerryTreeId(gSelectedObjectEvent)) == BERRY_STAGE_NO_BERRY)
        return TRUE;
    else
        return FALSE;
}

bool8 TryToWaterBerryTree(void)
{
    if (GetObjectEventScriptPointerPlayerFacing() != S_BerryTree)
        return FALSE;
    else
        return ObjectEventInteractionWaterBerryTree();
}

void ClearBerryTrees(void)
{
    int i;
    struct SaveBlock1 *saveBlock1 = &gSaveBlock1;
    struct BerryTree berryTree = gBlankBerryTree;

    for (i = 0; i < MAX_BERRY_TREES; i++)
        saveBlock1->berryTrees[i] = berryTree;
}

// when the player does not interact with the tree for a period of time, this is called
// to advance the grow state.
static bool32 BerryTreeGrow(struct BerryTree *tree)
{
    if (tree->growthSparkle)
        return FALSE;
    switch (tree->stage)
    {
    case BERRY_STAGE_NO_BERRY:
        return FALSE;
    case BERRY_STAGE_FLOWERING:
        tree->berryYield = CalcBerryYield(tree);
    case BERRY_STAGE_PLANTED:
    case BERRY_STAGE_SPROUTED:
    case BERRY_STAGE_TALLER:
        tree->stage++;
        break;
    case BERRY_STAGE_BERRIES:
        tree->watered1 = 0;
        tree->watered2 = 0;
        tree->watered3 = 0;
        tree->watered4 = 0;
        tree->berryYield = 0;
        tree->stage = BERRY_STAGE_SPROUTED;
        if (++tree->regrowthCount == BERRY_REGROW_LIMIT)
            *tree = gBlankBerryTree;
        break;
    }
    return TRUE;
}

void BerryTreeTimeUpdate(s32 minutesPassed)
{
    int i;
    struct BerryTree *tree;

    for (i = 0; i < MAX_BERRY_TREES; i++)
    {
        tree = &gSaveBlock1.berryTrees[i];

        if (tree->berry != BERRY_NONE && tree->stage != BERRY_STAGE_NO_BERRY && tree->growthSparkle == FALSE)
        {
            // the player has waited too long to water the berry. Reset the tree. This is because
            // if the berry state is not in the unwatered state, the tree will grow anyway despite this
            // check, which means BerryTreeGrow will handle the regrow process for this, removing the
            // need for this check. This only handles the unwatered soil state.
            if (minutesPassed >= GetStageDurationByBerryType(tree->berry) * 71)
            {
                *tree = gBlankBerryTree;
            }
            else
            {
                // because time is altered below, perhaps they thought it was unsafe to change it, even
                // though that is not how passed arguments behave.
                s32 time = minutesPassed;

                while (time != 0)
                {
                    if (tree->minutesUntilNextStage > time)
                    {
                        // its been X minutes since the last berry update, so update
                        // minutesUntilNextStage appropriately to match the time offset
                        // that has passed since the update.
                        tree->minutesUntilNextStage -= time;
                        break;
                    }
                    // perform the subtraction the other way around to get the number of minutes since
                    // the inferred stage update that occured, since minutesUntilNextStage is <= time.
                    // we may need this variable to simulate multiple berry cycles in the while loop.
                    time -= tree->minutesUntilNextStage;
                    tree->minutesUntilNextStage = GetStageDurationByBerryType(tree->berry); // since the tree was inferred to update, set the new minutesUntilNextStage.
                    if (BerryTreeGrow(tree) == FALSE)
                        break;
                    if (tree->stage == BERRY_STAGE_BERRIES)
                        tree->minutesUntilNextStage *= 4;
                }
            }
        }
    }
}

void PlantBerryTree(u8 id, u8 berry, u8 stage, bool8 sparkle)
{
    struct BerryTree *tree = GetBerryTreeInfo(id);

    *tree = gBlankBerryTree;
    tree->berry = berry;
    tree->minutesUntilNextStage = GetStageDurationByBerryType(berry);
    tree->stage = stage;
    if (stage == BERRY_STAGE_BERRIES)
    {
        tree->berryYield = CalcBerryYield(tree);
        tree->minutesUntilNextStage *= 4;
    }
    if (!sparkle)
    {
        tree->growthSparkle = TRUE;
    }
}

void RemoveBerryTree(u8 id)
{
    gSaveBlock1.berryTrees[id] = gBlankBerryTree;
}

u8 GetBerryTypeByBerryTreeId(u8 id)
{
    return gSaveBlock1.berryTrees[id].berry;
}

u8 GetStageByBerryTreeId(u8 id)
{
    return gSaveBlock1.berryTrees[id].stage;
}

u8 ItemIdToBerryType(u16 item)
{
    u16 berry = item - FIRST_BERRY_INDEX;

    if (berry > LAST_BERRY_INDEX - FIRST_BERRY_INDEX)
        return ITEM_TO_BERRY(FIRST_BERRY_INDEX);
    else
        return ITEM_TO_BERRY(item);
}

static u16 BerryTypeToItemId(u16 berry)
{
    u16 item = berry - 1;

    if (item > LAST_BERRY_INDEX - FIRST_BERRY_INDEX)
        return FIRST_BERRY_INDEX;
    else
        return berry + FIRST_BERRY_INDEX - 1;
}

void GetBerryNameByBerryType(u8 berry, u8 *string)
{
    memcpy(string, GetBerryInfo(berry)->name, BERRY_NAME_LENGTH);
    string[BERRY_NAME_LENGTH] = EOS;
}

void ResetBerryTreeSparkleFlag(u8 id)
{
    GetBerryTreeInfo(id)->growthSparkle = FALSE;
}

static u8 BerryTreeGetNumStagesWatered(struct BerryTree *tree)
{
    u8 count = 0;

    if (tree->watered1)
        count++;
    if (tree->watered2)
        count++;
    if (tree->watered3)
        count++;
    if (tree->watered4)
        count++;
    return count;
}

static u8 GetNumStagesWateredByBerryTreeId(u8 id)
{
    return BerryTreeGetNumStagesWatered(GetBerryTreeInfo(id));
}

static u8 CalcBerryYieldInternal(u16 max, u16 min, u8 water)
{
    u32 randMin;
    u32 randMax;
    u32 rand;
    u32 extraYield;

    // depending on if the player gave the tree plenty of water, berry yield will be affected proportionally.

    if (water == 0)
        return min;
    else
    {
        randMin = (max - min) * (water - 1);
        randMax = (max - min) * (water);
        rand = randMin + Random() % (randMax - randMin + 1);

        if ((rand % 4) > 1)
            extraYield = rand / 4 + 1;
        else
            extraYield = rand / 4;
        return extraYield + min;
    }
}

static u8 CalcBerryYield(struct BerryTree *tree)
{
    const struct Berry *berry = GetBerryInfo(tree->berry);
    u8 min = berry->minYield;
    u8 max = berry->maxYield;

    return CalcBerryYieldInternal(max, min, BerryTreeGetNumStagesWatered(tree));
}

static u8 GetBerryCountByBerryTreeId(u8 id)
{
    return gSaveBlock1.berryTrees[id].berryYield;
}

static u16 GetStageDurationByBerryType(u8 berry)
{
    return GetBerryInfo(berry)->stageDuration * 60;
}

void ObjectEventInteractionGetBerryTreeData(void)
{
    u8 id;
    u8 berry;
    u8 localId;
    u8 group;
    u8 num;

    id = ObjectEventGetBerryTreeId(gSelectedObjectEvent);
    berry = GetBerryTypeByBerryTreeId(id);
    ResetBerryTreeSparkleFlag(id);
    localId = gSpecialVar_LastTalked;
    num = gSaveBlock1.location.mapNum;
    group = gSaveBlock1.location.mapGroup;
    if (IsBerryTreeSparkling(localId, num, group))
    {
        // we cannot allow the player to grow/interact with the tree while the tree
        // is undergoing the sparkling effect, so set the special var to the sparkling
        // state and let the event script process the flag.
        gSpecialVar_0x8004 = BERRY_STAGE_SPARKLING;
    }
    else
        gSpecialVar_0x8004 = GetStageByBerryTreeId(id);
    gSpecialVar_0x8005 = GetNumStagesWateredByBerryTreeId(id);
    gSpecialVar_0x8006 = GetBerryCountByBerryTreeId(id);
    GetBerryNameByBerryType(berry, gStringVar1);
}

void Berry_FadeAndGoToBerryBagMenu(void)
{
    SetMainCallback2(CB2_ChooseBerry);
}

void ObjectEventInteractionPlantBerryTree(void)
{
    u8 berry = ItemIdToBerryType(gSpecialVar_ItemId);

    PlantBerryTree(ObjectEventGetBerryTreeId(gSelectedObjectEvent), berry, 1, TRUE);
    ObjectEventInteractionGetBerryTreeData();
}

void ObjectEventInteractionPickBerryTree(void)
{
    u8 id = ObjectEventGetBerryTreeId(gSelectedObjectEvent);
    u8 berry = GetBerryTypeByBerryTreeId(id);

    gSpecialVar_0x8004 = AddBagItem(BerryTypeToItemId(berry), GetBerryCountByBerryTreeId(id));
}

void ObjectEventInteractionRemoveBerryTree(void)
{
    RemoveBerryTree(ObjectEventGetBerryTreeId(gSelectedObjectEvent));
    sub_8060288(gSpecialVar_LastTalked, gSaveBlock1.location.mapNum, gSaveBlock1.location.mapGroup);
}

bool8 PlayerHasBerries(void)
{
    return IsBagPocketNonEmpty(BAG_BERRIES);
}

#if DEBUG
void debug_sub_80C33FC(const u8 *buffer, s32 value, u8 n)
{
    StringAppend(gStringVar4, buffer);
    ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_LEADING_ZEROS, n);
    StringAppend(gStringVar4, gStringVar1);
}

static const u8 gUnknown_Debug_083F7F9D[] = _("POS:");
static const u8 gUnknown_Debug_083F7FA2[] = _("\nTYPE:");
static const u8 gUnknown_Debug_083F7FA9[] = _("\nGROW:");
static const u8 gUnknown_Debug_083F7FB0[] = _("\nTIME:");
static const u8 gUnknown_Debug_083F7FB7[] = _("\nFCNT:");
static const u8 gUnknown_Debug_083F7FBE[] = _("\nSCNT:");
static const u8 gUnknown_Debug_083F7FC5[] = _("\nHOOK:");
static const u8 gUnknown_Debug_083F7FCC[] = _("\nWBIT:");
static const u8 gUnknown_Debug_083F7FD3[] = _("");

u8* DebugOpenBerryInfo(void)
{
    if (GetObjectEventScriptPointerPlayerFacing() != S_BerryTree)
    {
        return NULL;
    }
    else
    {
        u32 berryTreeId = ObjectEventGetBerryTreeId(gSelectedObjectEvent);
        struct BerryTree *berryTree = GetBerryTreeInfo(berryTreeId);
        s32 i;

        for (i = 0; i < 500; i++)
            gStringVar4[i] = EOS;

        debug_sub_80C33FC(gUnknown_Debug_083F7F9D, berryTreeId, 3);
        debug_sub_80C33FC(gUnknown_Debug_083F7FA2, berryTree->berry, 2);
        debug_sub_80C33FC(gUnknown_Debug_083F7FA9, berryTree->stage, 2);
        debug_sub_80C33FC(gUnknown_Debug_083F7FB0, berryTree->minutesUntilNextStage, 5);
        debug_sub_80C33FC(gUnknown_Debug_083F7FB7, berryTree->berryYield, 2);
        debug_sub_80C33FC(gUnknown_Debug_083F7FBE, berryTree->regrowthCount, 3);
        debug_sub_80C33FC(gUnknown_Debug_083F7FC5, berryTree->growthSparkle, 1);
        debug_sub_80C33FC(gUnknown_Debug_083F7FCC, berryTree->watered1, 1);
        debug_sub_80C33FC(gUnknown_Debug_083F7FD3, berryTree->watered2, 1);
        debug_sub_80C33FC(gUnknown_Debug_083F7FD3, berryTree->watered3, 1);
        debug_sub_80C33FC(gUnknown_Debug_083F7FD3, berryTree->watered4, 1);

        return gStringVar4;
    }
}

#endif

// whenever the player is not within view of the berry tree during its sparkle state, the
// sparkle state will be reset.
void ResetBerryTreeSparkleFlags(void)
{
    s16 cam_left;
    s16 cam_top;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    int i;

    GetCameraCoords(&cam_left, &cam_top);
    left = cam_left;
    top = cam_top + 3;
    right = cam_left + 14;
    bottom = top + 8;
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].active && gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            cam_left = gObjectEvents[i].currentCoords.x;
            cam_top = gObjectEvents[i].currentCoords.y;
            if (left <= cam_left && cam_left <= right && top <= cam_top && cam_top <= bottom)
                ResetBerryTreeSparkleFlag(gObjectEvents[i].trainerRange_berryTreeId);
        }
    }
}
